#ifndef SRC_ENGINE_INCLUDE_UINTA_FONT_H_
#define SRC_ENGINE_INCLUDE_UINTA_FONT_H_

#include <algorithm>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float4.hpp"
#include "stb/stb_truetype.h"
#include "uinta/status.h"
#include "uinta/texture.h"
#include "uinta/types.h"
#include "uinta/utils/alignment.h"

namespace uinta {

constexpr auto ElementsPerTextVertex = 9;
constexpr auto VerticesPerTextQuad = 6;

class Font {
 public:
  explicit Font(std::string path, i32 bmapWidth = 1024, i32 bmapHeight = 1024,
                f32 bmapLineSize = 128,
                const OpenGLApi* = OpenGLApiImpl::Instance()) noexcept;

  Font(const Font&) noexcept = delete;
  Font& operator=(const Font&) noexcept = delete;
  Font(Font&&) noexcept = delete;
  Font& operator=(Font&&) noexcept = delete;

  const Status& status() const noexcept { return status_; }

  StatusOr<stbtt_aligned_quad> quad(i8 c, f32* x, f32* y,
                                    f32 scale = 1) const noexcept;

  const Texture& texture() const noexcept { return texture_; }

  f32 ascent() const noexcept { return ascent_; }

  f32 descent() const noexcept { return descent_; }

  f32 lineGap() const noexcept { return lineGap_; }

  f32 lineSize() const noexcept { return lineSize_; }

  f32 scale() const noexcept { return scale_; }

  f32 spaceWidth() const noexcept { return spaceWidth_; }

 private:
  static constexpr size_t FirstChar = ' ';
  static constexpr size_t LastChar = '~';
  static constexpr size_t CharCount = LastChar - FirstChar + 1;

  Status status_;
  Texture texture_;
  std::array<stbtt_packedchar, CharCount> chars_;
  f32 ascent_;
  f32 descent_;
  f32 lineGap_;
  f32 lineSize_;
  f32 scale_;
  f32 spaceWidth_;
};

class TextMeshBuilder {
  // TODO: Convert to conventional `struct Flags final { ... }`:
  using Flags = u8;

  friend class TextAlignment;

 public:
  TextMeshBuilder(const Font* font, const std::string& text) noexcept
      : text_(text), lineSize_(font->lineSize()), font_(font) {}

  StatusOr<std::vector<f32>> build() const noexcept;

  TextMeshBuilder& align(Alignment alignment) noexcept {
    flags_ &= ~(_::Masks::Horizontal | _::Masks::Vertical);
    flags_ |= static_cast<Flags>(alignment);
    return *this;
  }

  // TODO:
  // TextMeshBuilder& kern(f32) noexcept;
  // TextMeshBuilder& charSpacing(f32) noexcept;
  // TextMeshBuilder& lineSpacing(f32) noexcept;
  // TextMeshBuilder& underline() noexcept;

  TextMeshBuilder& color(const glm::vec4& color) noexcept {
    color_ = color;
    return *this;
  }

  TextMeshBuilder& opacity(f32 opacity) noexcept {
    opacity_ = opacity;
    return *this;
  }

  TextMeshBuilder& text(const std::string& text) noexcept {
    text_ = text;
    return *this;
  }

  TextMeshBuilder& lineSize(f32 lineSize) noexcept {
    lineSize_ = lineSize;
    return *this;
  }

  TextMeshBuilder& dimensions(const glm::vec2& dimensions) noexcept {
    dimensions_.x =
        dimensions.x ? dimensions.x : std::numeric_limits<f32>::max();
    dimensions_.y =
        dimensions.y ? dimensions.y : std::numeric_limits<f32>::max();
    return *this;
  }

  TextMeshBuilder& position(const glm::vec2& position) noexcept {
    position_ = position;
    return *this;
  }

 private:
  struct Word {
    std::vector<stbtt_aligned_quad> quads;

    f32 calculateWidth() const noexcept {
      f32 width = 0;
      std::for_each(quads.begin(), quads.end(),
                    [&width](auto& quad) { width += quad.x1 - quad.x0; });
      return width;
    }
  };

  struct TextMeshGenerator {
    explicit TextMeshGenerator(const TextMeshBuilder* builder) noexcept
        : builder_(builder) {}

    virtual std::vector<f32>& generate(const std::vector<Word>&) noexcept = 0;

    virtual ~TextMeshGenerator() noexcept {}

   protected:
    const TextMeshBuilder* builder_;
    std::vector<f32> vertices_;

    void reserve(const std::vector<Word>&) noexcept;
  };

  struct BottomCenterTextMeshGenerator : TextMeshGenerator {
    explicit BottomCenterTextMeshGenerator(
        const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct BottomLeftTextMeshGenerator : TextMeshGenerator {
    explicit BottomLeftTextMeshGenerator(
        const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct BottomRightTextMeshGenerator : TextMeshGenerator {
    explicit BottomRightTextMeshGenerator(
        const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct MiddleCenterTextMeshGenerator : TextMeshGenerator {
    explicit MiddleCenterTextMeshGenerator(
        const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct MiddleLeftTextMeshGenerator : TextMeshGenerator {
    explicit MiddleLeftTextMeshGenerator(
        const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct MiddleRightTextMeshGenerator : TextMeshGenerator {
    explicit MiddleRightTextMeshGenerator(
        const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct TopCenterTextMeshGenerator : TextMeshGenerator {
    explicit TopCenterTextMeshGenerator(const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct TopLeftTextMeshGenerator : TextMeshGenerator {
    explicit TopLeftTextMeshGenerator(const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  struct TopRightTextMeshGenerator : TextMeshGenerator {
    explicit TopRightTextMeshGenerator(const TextMeshBuilder* builder) noexcept
        : TextMeshGenerator(builder) {}

    std::vector<f32>& generate(const std::vector<Word>&) noexcept override;
  };

  friend std::unique_ptr<TextMeshBuilder::TextMeshGenerator> findMeshGenerator(
      const TextMeshBuilder*) noexcept;

  std::string text_;
  glm::vec4 color_ = glm::vec4(1);
  glm::vec2 dimensions_ = glm::vec2(std::numeric_limits<f32>::max());
  glm::vec2 position_ = {0, 0};
  Flags flags_ = (_::Masks::Top << _::Shifts::Vertical) |
                 (_::Masks::Left << _::Shifts::Horizontal);
  f32 opacity_ = 1;
  f32 lineSize_;
  const Font* font_;

  bool isLeftAligned() const noexcept {
    return ((flags_ & _::Masks::Horizontal) >> _::Shifts::Horizontal) ==
           _::Masks::Left;
  }

  bool isRightAligned() const noexcept {
    return ((flags_ & _::Masks::Horizontal) >> _::Shifts::Horizontal) ==
           _::Masks::Right;
  }

  bool isCenterAligned() const noexcept {
    return ((flags_ & _::Masks::Horizontal) >> _::Shifts::Horizontal) ==
           _::Masks::Center;
  }

  bool isTopAligned() const noexcept {
    return ((flags_ & _::Masks::Vertical) >> _::Shifts::Vertical) ==
           _::Masks::Top;
  }

  bool isMiddleAligned() const noexcept {
    return ((flags_ & _::Masks::Vertical) >> _::Shifts::Vertical) ==
           _::Masks::Middle;
  }

  bool isBottomAligned() const noexcept {
    return ((flags_ & _::Masks::Vertical) >> _::Shifts::Vertical) ==
           _::Masks::Bottom;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_FONT_H_
