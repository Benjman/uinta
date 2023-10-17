#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <spdlog/fwd.h>
#include <stb/stb_truetype.h>

#include <array>
#include <glm/vec2.hpp>
#include <span>
#include <uinta/fwd.hpp>
#include <uinta/renderer.hpp>
#include <vector>

namespace uinta {

class Font {
 public:
  Font(const std::string& path, i32 bitmap_size = 256) : m_path(path), m_bitmap(pow(bitmap_size, 2)) {
    assert((bitmap_size & (bitmap_size - 1)) == 0 && "Bitmap size must be a power of 2.");
  }

  uinta_error_code load(FileManager& file_manager, spdlog::logger* logger) noexcept;

  stbtt_aligned_quad quad_info(char c, f32* x_pos, f32* y_pos) const;

  f32 ascent() const noexcept {
    return m_ascent;
  }

  std::span<const u8> bitmap() const noexcept {
    return m_bitmap;
  }

  constexpr size_t char_count() const noexcept {
    return m_chars.size();
  }

  f32 descent() const noexcept {
    return m_descent;
  }

  f32 line_gap() const noexcept {
    return m_line_gap;
  }

  f32 line_size() const noexcept {
    return m_line_size;
  }

  bool loaded() const noexcept {
    return m_scale;
  }

  const std::string& path() const noexcept {
    return m_path;
  }

  f32 scale() const noexcept {
    return m_scale;
  }

 private:
  static constexpr size_t FirstChar = ' ';
  static constexpr size_t LastChar = '~';
  static constexpr size_t CharCount = LastChar - FirstChar + 1;

  const std::string m_path;
  std::vector<u8> m_bitmap;
  std::array<stbtt_packedchar, CharCount> m_chars;
  f32 m_ascent = 0;
  f32 m_descent = 0;
  f32 m_line_gap = 0;
  f32 m_line_size = 47;
  f32 m_scale = 0;
};

}  // namespace uinta

#endif  // UINTA_FONT_H
