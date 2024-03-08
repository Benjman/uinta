#include "uinta/texture.h"

#include "absl/log/log.h"
#include "uinta/file.h"
#include "uinta/gl.h"

namespace uinta {

Texture::Texture(GLenum target, i32 width, i32 height, i32 channels, i32 filter,
                 i32 wrap, const OpenGLApi* gl) noexcept
    : target_(target),
      width_(width),
      height_(height),
      channels_(channels),
      filterMag_(filter),
      filterMin_(filter),
      wrap_(wrap),
      gl_(gl) {
  gl_->genTextures(1, &id_);
  TextureGuard guard(this);
  if (width_ && height_) setStorage(width, height);
  if (filterMag_) filtering(filterMag_, filterMin_);
  if (wrap_) wrapping(wrap_);
}

Status Texture::fromFile(std::string path, FileSystem* fs,
                         const StbImageApi* imageApi) noexcept {
  File file(path, fs);
  if (!file.status().ok()) return file.status();

  LOG(INFO) << "Found texture " << file.path();

  imageApi->setFlipVerticallyOnLoad(true);
  u8* data =
      imageApi->load(file.path().c_str(), &width_, &height_, &channels_, 0);

  TextureGuard guard(this);
  gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, data);
  gl_->generateMipmap(target_);
  imageApi->imageFree(data);

  return OkStatus();
}

}  // namespace uinta
