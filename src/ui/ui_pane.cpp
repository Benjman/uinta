#include <uinta/error.hpp>
#include <uinta/ui/ui_pane.hpp>

namespace uinta {

uinta_error_code UiPane::generate_mesh(std::span<f32> vtx_buffer, std::span<u32> idx_buffer) noexcept {
  const std::array<f32, 24> vtx = {
      m_top_left.x,     m_top_left.y,     m_top_left.z, m_color.r, m_color.g, m_color.b,
      m_top_left.x,     m_bottom_right.y, m_top_left.z, m_color.r, m_color.g, m_color.b,
      m_bottom_right.x, m_bottom_right.y, m_top_left.z, m_color.r, m_color.g, m_color.b,
      m_bottom_right.x, m_top_left.y,     m_top_left.z, m_color.r, m_color.g, m_color.b,
  };
  std::copy(vtx.begin(), vtx.end(), vtx_buffer.begin());
  constexpr std::array<u32, IndexCount> idx = {
      0, 1, 2, 2, 3, 0,
  };
  std::copy(idx.begin(), idx.end(), idx_buffer.begin());
  return SUCCESS_EC;
}

}  // namespace uinta
