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

Texture::~Texture() noexcept {
  gl_->deleteTextures(1, &id_);
  id_ = 0;
  width_ = 0;
  height_ = 0;
}

Status Texture::fromFile(std::string path,
                         std::shared_ptr<FileSystem> fs) noexcept {
  File file(path, fs);
  if (!file.status().ok()) return file.status();

  unsigned char* data =
      stbi_load(file.path().data(), &width_, &height_, &channels_, 0);

  TextureGuard guard(this);
  gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, data);
  gl_->generateMipmap(target_);
  stbi_image_free(data);
  return {};
}

void Texture::filtering(GLint min, GLint mag) noexcept {
  filterMin_ = min;
  filterMag_ = mag;
  gl_->texParameteri(target_, GL_TEXTURE_MIN_FILTER, min);
  gl_->texParameteri(target_, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::wrapping(GLint wrap) noexcept {
  wrap_ = wrap;
  gl_->texParameteri(target_, GL_TEXTURE_WRAP_S, wrap_);
  gl_->texParameteri(target_, GL_TEXTURE_WRAP_T, wrap_);
  if (target_ == GL_TEXTURE_3D)
    gl_->texParameteri(target_, GL_TEXTURE_WRAP_R, wrap_);
}

void Texture::setStorage(i32 width, i32 height) noexcept {
  width_ = width;
  height_ = height;
  gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, nullptr);
}

}  // namespace uinta
