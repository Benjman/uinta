#ifndef UINTA_FBO_HPP
#define UINTA_FBO_HPP

#include <uinta/gl/api.h>

namespace uinta {

struct Fbo {
  uint id = 0;

  uint renderTexture = 0;
  uint depthBuffer;
  uint height = 0;
  uint width = 0;

  Fbo(uint width = 0, uint height = 0) : width(width), height(height) {
  }
};

void initFbo(Fbo& fbo);

}  // namespace uinta

#endif  // UINTA_FBO_HPP
