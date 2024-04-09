#include "uinta/font.h"

#include "absl/log/log.h"
#include "uinta/file.h"
#include "uinta/lib/absl/strings.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#define STB_RECT_PACK_IMPLEMENTATION
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"
#pragma GCC diagnostic pop

namespace uinta {

Font::Font(std::string path, i32 bmapWidth, i32 bmapHeight, f32 bmapLineSize,
           const OpenGLApi* gl) noexcept
    : texture_(GL_TEXTURE_2D, bmapWidth, bmapHeight),
      lineSize_(bmapLineSize),
      gl_(gl) {
  LOG(INFO) << "Loading font: " << path;

  File file(path);
  status_ = file.status();
  if (!status_.ok()) return;

  stbtt_pack_context ctx;
  constexpr void* AllocContext = nullptr;
  constexpr i32 FontIndex = 0;
  constexpr i32 Padding = 1;
  constexpr i32 Stride = 0;
  constexpr i32 Channels = 1;

  std::vector<u8> bitmap;
  bitmap.reserve(texture_.width() * texture_.height() * Channels);

  if (!stbtt_PackBegin(&ctx, bitmap.data(), texture_.width(), texture_.height(),
                       Stride, Padding, AllocContext)) {
    status_ = InternalError("stbtt_PackBegin failure.");
    return;
  }

  if (!stbtt_PackFontRange(&ctx, file.readBinary().data(), FontIndex, lineSize_,
                           FirstChar, CharCount, chars_.data())) {
    status_ = InternalError("stbtt_PackFontRange failure.");
    return;
  }

  stbtt_fontinfo info;
  if (!stbtt_InitFont(&info, file.readBinary().data(), 0)) {
    status_ = InternalError("stbtt_InitFont failure.");
    return;
  }

  scale_ = stbtt_ScaleForPixelHeight(&info, lineSize_);
  i32 ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

  ascent_ = ascent * scale_;
  descent_ = descent * scale_;
  lineGap_ = lineGap * scale_;

  TextureGuard tg(&texture_);
  // TODO: Texture uploading functions
  gl_->texImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_.width(), texture_.height(),
                  0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
  texture_.filtering(GL_LINEAR, GL_LINEAR);

  LOG(INFO) << "Loaded font: " << path;
}

StatusOr<stbtt_aligned_quad> Font::quad(i8 c, f32* x, f32* y) const noexcept {
  if (c - FirstChar >= chars_.size())
    return OutOfRangeError(StrFormat("'%c' is out of bounds.", c));
  constexpr i32 AlignToInt = 0;
  stbtt_aligned_quad quad;
  stbtt_GetPackedQuad(chars_.data(), texture_.width(), texture_.height(),
                      c - FirstChar, x, y, &quad, AlignToInt);
  return quad;
}

}  // namespace uinta
