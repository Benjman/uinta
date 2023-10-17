#ifndef UINTA_TEXT_HPP
#define UINTA_TEXT_HPP

#include <uinta/types.h>

#include <span>
#include <string>
#include <uinta/font.hpp>

namespace uinta {

class TextMesh {
 public:
  TextMesh(const std::string& text, f32 line_size, f32 max_width = 0)
      : m_text(text), m_max_width(max_width), m_line_size(line_size) {
  }

  uinta_error_code generate(const Font& font, std::span<f32> vertex_buffer, std::span<u32> index_buffer,
                            spdlog::logger* logger) noexcept;

  glm::vec2 calc_size(const Font& font) const noexcept;

  size_t vertex_buffer_size() const noexcept {
    constexpr size_t PositionsPerChar = 3;  // x,y,z
    constexpr size_t UVCoordsPerChar = 2;   // u,v
    constexpr size_t ColorsPerChar = 3;     // r,g,b
    constexpr size_t PointsPerChar = 4;     // p0..p3
    return (PositionsPerChar + UVCoordsPerChar + ColorsPerChar) * PointsPerChar * m_text.size();
  }

  size_t index_buffer_size() const noexcept {
    constexpr size_t IndicesPerChar = 6;
    return IndicesPerChar * m_text.size();
  }

  f32 height() const noexcept {
    return m_height;
  }

  f32 line_size() const noexcept {
    return m_line_size;
  }

  f32 max_width() const noexcept {
    return m_max_width;
  }

  const std::string& text() const noexcept {
    return m_text;
  }

  f32 width() const noexcept {
    return m_width;
  }

 private:
  const std::string m_text;
  const f32 m_max_width;
  const f32 m_line_size;
  f32 m_height = 0;
  f32 m_width = 0;

  uinta_error_code validate(const Font& font, std::span<f32> vertex_buffer, std::span<u32> index_buffer,
                            spdlog::logger* logger) const noexcept;
};

}  // namespace uinta

#endif  // UINTA_TEXT_HP
