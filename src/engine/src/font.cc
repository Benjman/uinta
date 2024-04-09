#include "uinta/font.h"

#include <algorithm>
#include <cassert>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/file.h"

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
    : texture_(GL_TEXTURE_2D, bmapWidth, bmapHeight, 0, 0, 0, gl),
      lineSize_(bmapLineSize) {
  assert(lineSize_ && "Line size cannot be zero.");

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
  gl->texImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_.width(), texture_.height(),
                 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
  texture_.filtering(GL_LINEAR, GL_LINEAR);

  f32 x, y;
  auto quad = this->quad(' ', &x, &y);
  spaceWidth_ = quad.value().x1 - quad.value().x0;

  LOG(INFO) << absl::StrFormat("Loaded font: %s", file.path());
}

StatusOr<stbtt_aligned_quad> Font::quad(i8 c, f32* x, f32* y,
                                        f32 scale) const noexcept {
  if (c - FirstChar >= chars_.size())
    return OutOfRangeError(absl::StrFormat("'%c' is out of bounds.", c));
  constexpr i32 AlignToInt = 0;
  stbtt_aligned_quad quad;
  stbtt_GetPackedQuad(chars_.data(), texture_.width(), texture_.height(),
                      c - FirstChar, x, y, &quad, AlignToInt);
  quad.x0 *= scale;
  quad.x1 *= scale;
  quad.y0 *= scale;
  quad.y1 *= scale;
  return quad;
}

void TextMeshBuilder::TextMeshGenerator::reserve(
    const std::vector<Word>& words) noexcept {
  auto count = 0;
  std::for_each(words.begin(), words.end(),
                [&count](const auto& word) { count += word.quads.size(); });
  vertices_.reserve(count * ElementsPerTextVertex * VerticesPerTextQuad);
}

std::vector<f32>& TextMeshBuilder::BottomCenterTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "BottomCenterTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::BottomLeftTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "BottomLeftTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::BottomRightTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "BottomRightTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::MiddleCenterTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "MiddleCenterTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::MiddleLeftTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "MiddleLeftTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::MiddleRightTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "MiddleRightTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::TopCenterTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "TopCenterTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::TopLeftTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  reserve(words);
  const auto& color = builder_->color_;
  for (auto& word : words) {
    for (auto& quad : word.quads) {
      // clang-format off
      std::array<f32, VerticesPerTextQuad * ElementsPerTextVertex> data = {                 // NOLINT
        quad.x1, quad.y0, 0,     color.r, color.g, color.b, color.a,     quad.s1, quad.t0,  // NOLINT
        quad.x0, quad.y0, 0,     color.r, color.g, color.b, color.a,     quad.s0, quad.t0,  // NOLINT
        quad.x0, quad.y1, 0,     color.r, color.g, color.b, color.a,     quad.s0, quad.t1,  // NOLINT
        quad.x1, quad.y0, 0,     color.r, color.g, color.b, color.a,     quad.s1, quad.t0,  // NOLINT
        quad.x0, quad.y1, 0,     color.r, color.g, color.b, color.a,     quad.s0, quad.t1,  // NOLINT
        quad.x1, quad.y1, 0,     color.r, color.g, color.b, color.a,     quad.s1, quad.t1,  // NOLINT
      };
      // clang-format on
      vertices_.insert(vertices_.end(), data.begin(), data.end());
    }
  }
  return vertices_;
}

std::vector<f32>& TextMeshBuilder::TopRightTextMeshGenerator::generate(
    const std::vector<Word>& words) noexcept {
  LOG(FATAL) << "TopRightTextMeshGenerator unimplemented.";
  reserve(words);
  return vertices_;
}

std::unique_ptr<TextMeshBuilder::TextMeshGenerator> findMeshGenerator(
    const TextMeshBuilder* builder) noexcept {
  if (builder->isBottomAligned()) {
    if (builder->isCenterAligned()) {
      return std::make_unique<TextMeshBuilder::BottomCenterTextMeshGenerator>(
          builder);
    } else if (builder->isLeftAligned()) {
      return std::make_unique<TextMeshBuilder::BottomLeftTextMeshGenerator>(
          builder);
    } else if (builder->isRightAligned()) {
      return std::make_unique<TextMeshBuilder::BottomRightTextMeshGenerator>(
          builder);
    } else {
      LOG(FATAL) << "Invalid alignment.";
    }
  } else if (builder->isMiddleAligned()) {
    if (builder->isCenterAligned()) {
      return std::make_unique<TextMeshBuilder::MiddleCenterTextMeshGenerator>(
          builder);
    } else if (builder->isLeftAligned()) {
      return std::make_unique<TextMeshBuilder::MiddleLeftTextMeshGenerator>(
          builder);
    } else if (builder->isRightAligned()) {
      return std::make_unique<TextMeshBuilder::MiddleRightTextMeshGenerator>(
          builder);
    } else {
      LOG(FATAL) << "Invalid alignment.";
    }
  } else if (builder->isTopAligned()) {
    if (builder->isCenterAligned()) {
      return std::make_unique<TextMeshBuilder::TopCenterTextMeshGenerator>(
          builder);
    } else if (builder->isLeftAligned()) {
      return std::make_unique<TextMeshBuilder::TopLeftTextMeshGenerator>(
          builder);
    } else if (builder->isRightAligned()) {
      return std::make_unique<TextMeshBuilder::TopRightTextMeshGenerator>(
          builder);
    } else {
      LOG(FATAL) << "Invalid alignment.";
    }
  } else {
    LOG(FATAL) << "Invalid alignment.";
  }
}

StatusOr<std::vector<f32>> TextMeshBuilder::build() const noexcept {
  std::vector<Word> words;
  auto* word = &words.emplace_back();
  auto scale = lineSize_ ? lineSize_ / font_->lineSize() : 1;
  auto width = 0.0;
  auto x = position_.x;
  auto y = position_.y + lineSize_ - font_->descent() * (1 - scale);
  for (auto c : text_) {
    if (c == '\n') {
      width = 0;
      x = position_.x;
      y += font_->lineSize();
      word = &words.emplace_back();
      continue;
    }
    auto quadStatus = font_->quad(c, &x, &y, scale);
    if (!quadStatus.ok()) return quadStatus.status();
    if (c == ' ') {
      auto wordWidth = word->calculateWidth();
      width += wordWidth;
      if (dimensions_.x && dimensions_.x < width) {
        width = 0;
        x = position_.x;
        y += font_->lineSize();
      }
      word = &words.emplace_back();
      continue;
    }
    word->quads.emplace_back(quadStatus.value());
  }
  return findMeshGenerator(this)->generate(words);
}

}  // namespace uinta
