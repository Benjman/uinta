#include "uinta/texture.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include "uinta/file.h"
#include "uinta/gl.h"

namespace uinta {

namespace {

TextureFormat LegacyRgbFormat() noexcept {
  // Preserve the original behavior of the legacy constructor: mutable RGB
  // storage (unsized internalformat) uploaded via glTexImage2D.
  TextureFormat f;
  f.internalFormat = GL_RGB;
  f.pixelFormat = GL_RGB;
  f.pixelType = GL_UNSIGNED_BYTE;
  f.levels = 1;
  f.immutable = false;
  return f;
}

}  // namespace

Texture::Texture(GLenum target, i32 width, i32 height, i32 channels, i32 filter,
                 i32 wrap, const OpenGLApi* gl) noexcept
    : target_(target),
      width_(width),
      height_(height),
      channels_(channels),
      filterMag_(filter),
      filterMin_(filter),
      wrap_(wrap),
      format_(LegacyRgbFormat()),
      gl_(gl) {
  gl_->genTextures(1, &id_);
  TextureGuard guard(this);
  if ((width_ != 0) && (height_ != 0)) {
    setStorage(width, height);
  }
  if (filterMag_ != 0) {
    filtering(filterMin_, filterMag_);
  }
  if (wrap_ != 0) {
    wrapping(wrap_);
  }
}

Texture::Texture(GLenum target, i32 width, i32 height,
                 const TextureFormat& format, i32 filterMin, i32 filterMag,
                 i32 wrap, const OpenGLApi* gl) noexcept
    : target_(target),
      width_(0),
      height_(0),
      channels_(0),
      filterMag_(filterMag),
      filterMin_(filterMin),
      wrap_(wrap),
      format_(format),
      gl_(gl) {
  gl_->genTextures(1, &id_);
  TextureGuard guard(this);
  if ((width != 0) && (height != 0)) {
    setStorage(width, height);
  }
  if (filterMag_ != 0 || filterMin_ != 0) {
    filtering(filterMin_, filterMag_);
  }
  if (wrap_ != 0) {
    wrapping(wrap_);
  }
}

void Texture::setStorage(i32 width, i32 height) noexcept {
  width_ = width;
  height_ = height;
  if (format_.immutable) {
    gl_->texStorage2D(target_, static_cast<GLsizei>(format_.levels),
                      format_.internalFormat, width_, height_);
  } else {
    gl_->texImage2D(target_, 0, static_cast<GLint>(format_.internalFormat),
                    width_, height_, 0, format_.pixelFormat, format_.pixelType,
                    nullptr);
  }
}

void Texture::resize(i32 width, i32 height) noexcept {
  if (width == width_ && height == height_) {
    return;
  }
  // For immutable storage we must allocate a fresh texture object since
  // glTexStorage* can only be invoked once per object. Regenerate the id.
  TextureGuard guard(this);
  if (format_.immutable) {
    gl_->deleteTextures(1, &id_);
    gl_->genTextures(1, &id_);
    gl_->bindTexture(target_, id_);
    setStorage(width, height);
    if (filterMag_ != 0 || filterMin_ != 0) {
      filtering(filterMin_, filterMag_);
    }
    if (wrap_ != 0) {
      wrapping(wrap_);
    }
  } else {
    setStorage(width, height);
  }
}

Status Texture::fromFile(const std::filesystem::path& path,
                         const StbImageApi* imageApi) noexcept {
  auto resolvedPath = FindFile(path);
  if (!resolvedPath.has_value()) {
    return NotFoundError(absl::StrFormat("Texture file not found: %s", path));
  }

  LOG(INFO) << "Found texture " << resolvedPath.value().string();

  imageApi->setFlipVerticallyOnLoad(true);
  u8* data = imageApi->load(resolvedPath.value().c_str(), &width_, &height_,
                            &channels_, 0);

  TextureGuard guard(this);
  gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, data);
  gl_->generateMipmap(target_);
  imageApi->imageFree(data);

  return OkStatus();
}

}  // namespace uinta
