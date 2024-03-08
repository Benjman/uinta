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

Texture::Texture(Texture&& other) noexcept
    : id_(other.id_),
      target_(other.target_),
      width_(other.width_),
      height_(other.height_),
      channels_(other.channels_),
      filterMag_(other.filterMag_),
      filterMin_(other.filterMin_),
      wrap_(other.wrap_),
      gl_(other.gl_) {
  other.id_ = 0;
  other.target_ = GL_NONE;
  other.width_ = 0;
  other.height_ = 0;
  other.channels_ = 0;
  other.filterMin_ = 0;
  other.filterMag_ = 0;
  other.wrap_ = 0;
  other.gl_ = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept {
  if (this != &other) {
    gl_->deleteTextures(1, &id_);
    id_ = other.id_;
    target_ = other.target_;
    width_ = other.width_;
    height_ = other.height_;
    channels_ = other.channels_;
    gl_ = other.gl_;
    filterMin_ = other.filterMin_;
    filterMag_ = other.filterMag_;
    wrap_ = other.wrap_;

    other.id_ = 0;
    other.target_ = GL_NONE;
    other.width_ = 0;
    other.height_ = 0;
    other.channels_ = 0;
    other.filterMin_ = 0;
    other.filterMag_ = 0;
    other.wrap_ = 0;
    other.gl_ = nullptr;
  }
  return *this;
}

Status Texture::fromFile(std::string path,
                         std::shared_ptr<FileSystem> fs) noexcept {
  Status status;
  auto file = File(path, fs);
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
  TextureGuard guard(this);
  gl_->texParameteri(target_, GL_TEXTURE_MIN_FILTER, min);
  gl_->texParameteri(target_, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::wrapping(GLint wrap) noexcept {
  TextureGuard guard(this);
  wrap_ = wrap;
  gl_->texParameteri(target_, GL_TEXTURE_WRAP_S, wrap_);
  gl_->texParameteri(target_, GL_TEXTURE_WRAP_T, wrap_);
  if (target_ == GL_TEXTURE_3D)
    gl_->texParameteri(target_, GL_TEXTURE_WRAP_R, wrap_);
}

void Texture::setStorage(i32 width, i32 height) noexcept {
  TextureGuard guard(this);
  width_ = width;
  height_ = height;
  gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, nullptr);
}

}  // namespace uinta
