#include <uinta/error.hpp>
#include <uinta/logging.hpp>
#include <uinta/text.hpp>

namespace uinta {

enum class error : error_t {
  VertexBufferSizeTooSmall = 10,
  IndexBufferSizeTooSmall = 11,
  QuadStoreSizeTooSmall = 12,
  InvalidFontSize = 13,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::VertexBufferSizeTooSmall), "Vertex buffer size is insufficient."},
    {static_cast<uinta_error_code_t>(error::IndexBufferSizeTooSmall), "Index buffer size is insufficient."},
    {static_cast<uinta_error_code_t>(error::QuadStoreSizeTooSmall), "Quads storage size is insufficient."},
    {static_cast<uinta_error_code_t>(error::InvalidFontSize), "Invalid font size."},
};

UINTA_ERROR_FRAMEWORK(Scene, errorMessages);

uinta_error_code TextMesh::validate(const Font& font, std::span<f32> vertex_buffer, std::span<u32> index_buffer,
                                    spdlog::logger* logger) const noexcept {
  if (vertex_buffer.size() < vertex_buffer_size()) {
    if (logger)
      SPDLOG_LOGGER_ERROR(logger, "{}\n\tInput text: '{}'\n\tInput font: '{}'",
                          errorMessages.at(static_cast<error_t>(error::VertexBufferSizeTooSmall)), text(), font.path());
    return make_error(error::VertexBufferSizeTooSmall);
  }
  if (index_buffer.size() < index_buffer_size()) {
    if (logger)
      SPDLOG_LOGGER_ERROR(logger, "{}\n\tInput text: '{}'\n\tInput font: '{}'",
                          errorMessages.at(static_cast<error_t>(error::IndexBufferSizeTooSmall)), text(), font.path());
    return make_error(error::IndexBufferSizeTooSmall);
  }
  if (!font.loaded()) {
    if (logger)
      SPDLOG_LOGGER_ERROR(logger, "{}\n\tInput text: '{}'\n\tInput font: '{}'",
                          errorMessages.at(static_cast<error_t>(error::InvalidFontSize)), text(), font.path());
    return make_error(error::InvalidFontSize);
  }
  return SUCCESS_EC;
}

uinta_error_code TextMesh::generate(const Font& font, std::span<f32> vertex_buffer, std::span<u32> index_buffer,
                                    spdlog::logger* logger) noexcept {
  if (auto error = validate(font, vertex_buffer, index_buffer, logger); error) return error;
  f32 scale = m_line_size / font.line_size();
  f32 x = 0;
  f32 y = font.ascent();
  f32 z_layer = 0;
  size_t index_cursor = 0;
  auto vertex_itr = vertex_buffer.begin();
  auto index_itr = index_buffer.begin();
  std::array<f32, 32> vertex_data;
  for (size_t i = 0; i < m_text.size(); ++i) {
    const auto c = m_text.at(i);
    const auto quad = font.quad_info(c, &x, &y);
    vertex_data = {
        quad.x0 * scale, quad.y0 * scale, z_layer, quad.s0, quad.t0, 1, 1, 1,
        quad.x0 * scale, quad.y1 * scale, z_layer, quad.s0, quad.t1, 1, 1, 1,
        quad.x1 * scale, quad.y1 * scale, z_layer, quad.s1, quad.t1, 1, 1, 1,
        quad.x1 * scale, quad.y0 * scale, z_layer, quad.s1, quad.t0, 1, 1, 1,
    };
    std::copy(vertex_data.begin(), vertex_data.end(), vertex_itr);
    std::advance(vertex_itr, vertex_data.size());
    const u32 base_index = index_cursor++ * 4;
    *index_itr++ = base_index + 0;
    *index_itr++ = base_index + 1;
    *index_itr++ = base_index + 2;
    *index_itr++ = base_index + 2;
    *index_itr++ = base_index + 3;
    *index_itr++ = base_index + 0;
  }
  m_width = x;
  m_height = y;
  return SUCCESS_EC;
}

glm::vec2 TextMesh::calc_size(const Font& font) const noexcept {
  glm::vec2 result;
  for (size_t i = 0; i < m_text.size(); ++i) font.quad_info(m_text.at(i), &result.x, &result.y);
  return result;
}

}  // namespace uinta
