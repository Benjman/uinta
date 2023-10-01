#ifndef UINTA_FBO_HPP
#define UINTA_FBO_HPP

#include <uinta/fwd.hpp>

namespace uinta {

struct Fbo {
  u32 id = 0;

  u32 renderTexture = 0;
  u32 depthBuffer;
  u32 height = 0;
  u32 width = 0;

  Fbo(u32 width = 0, u32 height = 0) : height(height), width(width) {
  }
};

uinta_error_code initFbo(Fbo& fbo);

}  // namespace uinta

#endif  // UINTA_FBO_HPP
