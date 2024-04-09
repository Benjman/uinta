#ifndef SRC_ENGINE_INCLUDE_UINTA_FONT_H_
#define SRC_ENGINE_INCLUDE_UINTA_FONT_H_

#include <string>

#include "stb/stb_truetype.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/status.h"
#include "uinta/texture.h"
#include "uinta/types.h"

namespace uinta {

class Font {
 public:
  explicit Font(std::string path, i32 bmapWidth = 256, i32 bmapHeight = 256,
                f32 bmapLineSize = 47,
                const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  Font(const Font&) noexcept = delete;
  Font& operator=(const Font&) noexcept = delete;
  Font(Font&&) noexcept = delete;
  Font& operator=(Font&&) noexcept = delete;

  const Status& status() const noexcept { return status_; }

  StatusOr<stbtt_aligned_quad> quad(i8 c, f32* x, f32* y) const noexcept;

  const Texture& texture() const noexcept { return texture_; }

 private:
  static constexpr size_t FirstChar = ' ';
  static constexpr size_t LastChar = '~';
  static constexpr size_t CharCount = LastChar - FirstChar + 1;

  Status status_;
  Texture texture_;
  std::array<stbtt_packedchar, CharCount> chars_;
  f32 lineSize_;
  f32 scale_;
  f32 ascent_;
  f32 descent_;
  f32 lineGap_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_FONT_H_
