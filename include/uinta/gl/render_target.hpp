#ifndef UINTA_RENDER_TARGET_HPP
#define UINTA_RENDER_TARGET_HPP

#include <uinta/gl/api.hpp>

namespace uinta {

class Vao;

struct RenderTarget {
  const Vao* vao;
  GLsizei count;
  const void* offset;

  RenderTarget(const Vao* vao = nullptr, GLsizei count = 0, const void* offset = 0) : vao(vao), count(count), offset(offset) {
  }

  RenderTarget(const RenderTarget& other) {
    *this = other;
  }

  RenderTarget& operator=(const RenderTarget& rhs) {
    vao = rhs.vao;
    count = rhs.count;
    offset = rhs.offset;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_RENDER_TARGET_HPP
