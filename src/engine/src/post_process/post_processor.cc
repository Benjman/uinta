#include "uinta/post_process/post_processor.h"

#include <absl/log/log.h>

#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "uinta/camera/camera_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

PostProcessor::PostProcessor(Engine* engine) noexcept
    : serviceHandle_(engine, this),
      engine_(engine),
      gl_(engine->gl()),
      blit_(engine->gl()),
      fullscreenVao_(engine->gl()),
      pool_(engine->gl()) {
  // Initial dimensions / camera params from the current window state and
  // viewport manager.
  if (auto* platform = engine_->platform();
      platform != nullptr && platform->window() != nullptr) {
    width_ = std::max<u32>(1, platform->window()->width());
    height_ = std::max<u32>(1, platform->window()->height());
  }

  if (auto* vp = engine_->service<ViewportManager>()) {
    near_ = vp->near();
    far_ = vp->far();
    projection_ = vp->projectionMatrix();
    vp->addListener<ViewportEvent::ProjectionMatrixUpdated>(
        [this](const ProjectionMatrixUpdateEvent& event) {
          projection_ = event.projection;
          if (auto* vp = engine_->service<ViewportManager>()) {
            near_ = vp->near();
            far_ = vp->far();
          }
        });
  }

  if (auto* cm = engine_->service<CameraManager>(); cm != nullptr) {
    cm->addListener<CameraEvent::ViewMatrixUpdated>(
        [&](const ViewMatrixUpdateEvent& e) { view_ = e.view; });
  }

  engine_->dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
      [this](const ViewportSizeChange& e) {
        resize(std::max<u32>(1, e.width), std::max<u32>(1, e.height));
      });

  engine_->dispatchers()->addListener<EngineEvent::RenderLayerChange>(
      [this](const RenderLayerChange& e) {
        // The engine dispatches RenderLayerChange when transitioning between
        // scenes of different layers. The Simulation layer is always first
        // (engine.h asserts this) and does not fire an event on entry, so
        // Simulation begin-of-frame binding is handled by a PreRender hook on
        // the owning scene. Exit-of-Simulation happens on the Simulation ->
        // {UI,Debug} transition, which is our run-chain hook.
        if (e.layer == SceneLayer::UI || e.layer == SceneLayer::Debug) {
          endSimulation();
        }
      });

  engine_->dispatchers()->addListener<EngineEvent::RenderComplete>(
      [this](const RenderComplete& e) { runtime_ = e.runtime; });

  allocateTargets();
}

void PostProcessor::removePass(const PostProcessPass* pass) noexcept {
  auto it =
      std::ranges::find_if(passes_.begin(), passes_.end(),
                           [pass](const std::unique_ptr<PostProcessPass>& p) {
                             return p.get() == pass;
                           });
  if (it != passes_.end()) {
    passes_.erase(it);
  }
}

void PostProcessor::resize(u32 width, u32 height) noexcept {
  if (width == width_ && height == height_ && initialized_) {
    return;
  }
  width_ = width;
  height_ = height;
  pool_.invalidate();
  allocateTargets();
}

void PostProcessor::allocateTargets() noexcept {
  sceneTarget_ = std::make_unique<RenderTarget>(
      gl_, width_, height_, RenderTargetFormats::Hdr(),
      RenderTargetFormats::Depth24(), "post_process::sceneTarget");
  for (u32 i = 0; i < pingPong_.size(); ++i) {
    pingPong_[i] = std::make_unique<RenderTarget>(
        gl_, width_, height_, RenderTargetFormats::Hdr(), std::nullopt,
        "post_process::pingPong[" + std::to_string(i) + "]");
  }
  initialized_ = true;
}

void PostProcessor::beginSimulation() noexcept {
  if (sceneTarget_ == nullptr || sceneTarget_->fbo() == nullptr) {
    return;
  }
  gl_->bindFramebuffer(GL_FRAMEBUFFER, sceneTarget_->fbo()->id());
  gl_->viewport(0, 0, static_cast<GLsizei>(width_),
                static_cast<GLsizei>(height_));
  gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessBeginComponent::update(time_t /*unused*/) noexcept {
  if (processor_ != nullptr) {
    processor_->beginSimulation();
  }
}

void PostProcessor::endSimulation() noexcept {
  if (sceneTarget_ == nullptr || sceneTarget_->fbo() == nullptr) {
    return;
  }

  // Collect enabled passes.
  std::vector<PostProcessPass*> active;
  active.reserve(passes_.size());
  for (auto& pass : passes_) {
    if (pass->enabled()) {
      active.push_back(pass.get());
    }
  }

  // Depth test off for the whole chain; disable depth writes too to be safe.
  DepthTestGuard depthGuard(false, gl_);
  DepthMaskGuard depthMaskGuard(GL_FALSE, gl_);

  if (active.empty()) {
    // Fallback: blit simulation color to the default framebuffer.
    blit_.draw(sceneTarget_->color(), nullptr, width_, height_);
    gl_->bindFramebuffer(GL_FRAMEBUFFER, 0);
    pool_.resetActive();
    return;
  }

  const glm::mat4 invView = glm::inverse(view_);
  const glm::mat4 invProjection = glm::inverse(projection_);
  const glm::mat4 invViewProjection = glm::inverse(projection_ * view_);

  // Build and execute the chain.
  const Texture* sourceColor = sceneTarget_->color();
  for (size_t i = 0; i < active.size(); ++i) {
    const bool isLast = (i + 1 == active.size());
    const size_t pingIndex = i & 1;

    PassContext ctx;
    ctx.gl = gl_;
    ctx.fullscreenVao = &fullscreenVao_;
    ctx.blit = &blit_;
    ctx.width = width_;
    ctx.height = height_;
    ctx.near = near_;
    ctx.far = far_;
    ctx.runtime = runtime_;
    ctx.sceneTarget = sceneTarget_.get();
    ctx.input = sourceColor;
    ctx.output = isLast ? nullptr : pingPong_[pingIndex]->fbo();
    ctx.view = view_;
    ctx.projection = projection_;
    ctx.invView = invView;
    ctx.invProjection = invProjection;
    ctx.invViewProjection = invViewProjection;
    ctx.pool = &pool_;

    active[i]->execute(ctx);

    if (!isLast) {
      sourceColor = pingPong_[pingIndex]->color();
    }
  }

  gl_->bindFramebuffer(GL_FRAMEBUFFER, 0);
  pool_.resetActive();
}

}  // namespace uinta
