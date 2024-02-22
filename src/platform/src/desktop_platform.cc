#include "uinta/desktop_platform.h"

#include "absl/log/log.h"
#include "uinta/desktop_window.h"
#include "uinta/glfw_platform_api.h"
#include "uinta/window.h"

namespace uinta {

DesktopPlatform::DesktopPlatform(WindowConfig config) noexcept
    : api_(GlfwPlatformApi::GetInstance()) {
  if (auto status = api_->initOpenGL(); !status.ok()) {
    return;
  }

  auto monitorsStatus = api_->findMonitors();
  if (!monitorsStatus.ok()) {
    return;
  }
  monitors_ = monitorsStatus.value();

  window_ = std::make_unique<DesktopWindow>(this, config);
  LOG(INFO) << "Initialized DesktopPlatform.";
}

Status DesktopPlatform::clearFrontBuffer(f32 r, f32 g, f32 b, f32 a,
                                         bitflag clearMask) const noexcept {
  return api_->clearFrontBuffer(r, g, b, a, clearMask);
}

Status DesktopPlatform::pollEvents() const noexcept {
  return api_->pollEvents();
}

StatusOr<f64> DesktopPlatform::runtime() const noexcept {
  return api_->runtime();
}

Status DesktopPlatform::swapBuffers() const noexcept {
  return api_->swapBuffers(window());
}

void DesktopPlatform::onCloseRequest(const CloseRequestFunc& func) noexcept {
  api_->onCloseRequest(func);
}

void DesktopPlatform::onDebugMessage(const DebugMessageFunc& func) noexcept {
  api_->onDebugMessage(func);
}

void DesktopPlatform::onError(const ErrorFunc& func) noexcept {
  api_->onError(func);
}

void DesktopPlatform::onFramebufferSize(
    const FramebufferSizeFunc& func) noexcept {
  api_->onFramebufferSize(func);
}

StatusOr<void*> DesktopPlatform::createWindow(const Window* window) noexcept {
  return api_->createWindow(window);
}

Status DesktopPlatform::destroy(const Window* window) const noexcept {
  return api_->destroy(window);
}

Status DesktopPlatform::registerInputHandlers(Input* input) const noexcept {
  return api_->registerInputHandlers(window_.get(), input);
}

Status DesktopPlatform::setWindowPosition(void* window, f32 x,
                                          f32 y) const noexcept {
  return api_->setWindowPosition(window, x, y);
}

}  // namespace uinta
