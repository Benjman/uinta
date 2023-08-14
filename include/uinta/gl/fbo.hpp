#ifndef UINTA_FBO_HPP
#define UINTA_FBO_HPP

#include <uinta/gl/api.h>

namespace uinta {

struct Fbo {
  GLuint id = 0;

  GLuint renderTexture = 0;
  GLuint depthBuffer;
  GLuint height = 0;
  GLuint width = 0;

  Fbo(GLuint width = 0, GLuint height = 0) : width(width), height(height) {
  }
};

void initFbo(Fbo& fbo);

}  // namespace uinta

#endif  // UINTA_FBO_HPP
