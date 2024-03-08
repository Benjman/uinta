#include "uinta/texture.h"

#include "uinta/file.h"
#include "uinta/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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

Status Texture::fromFile(std::string path,
                         std::shared_ptr<FileSystem> fs) noexcept {
  File file(path, fs);
  if (!file.status().ok()) return file.status();

  u8* data = stbi_load(file.path().data(), &width_, &height_, &channels_, 0);

  TextureGuard guard(this);
  gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, data);
  gl_->generateMipmap(target_);
  stbi_image_free(data);
  return OkStatus();
}

}  // namespace uinta
