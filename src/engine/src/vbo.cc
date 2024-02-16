#include "uinta/vbo.h"

namespace uinta {

VboGuard::VboGuard(const Vbo* vbo) noexcept
    : target_(vbo->target()), gl_(vbo->gl_) {
  vbo->gl_->bindBuffer(vbo->target_, vbo->id_);
}

VboGuard::~VboGuard() noexcept { gl_->bindBuffer(target_, 0); }

}  // namespace uinta
