#include "uinta/vbo.h"

namespace uinta {

VboGuard::VboGuard(const Vbo* vbo, bool isActive) noexcept : vbo_(vbo) {
  if (isActive) activate();
}

VboGuard::~VboGuard() noexcept {
  if (isActive_) vbo_->gl_->bindBuffer(vbo_->target_, 0);
}

void VboGuard::activate() noexcept {
  vbo_->gl_->bindBuffer(vbo_->target_, vbo_->id_);
  isActive_ = true;
}

}  // namespace uinta
