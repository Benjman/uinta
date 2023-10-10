#ifndef UINTA_UI_PANE_HPP
#define UINTA_UI_PANE_HPP

#include <array>
#include <uinta/ui/ui_element.hpp>

namespace uinta {

class UiPane : public UiElement {
 public:
  UiPane(const glm::vec3 position = {0, 0, 0}, const glm::vec3 size = {0, 0, 0}, const Anchor anchor = Anchor()) noexcept
      : UiElement(position, size, anchor) {
  }

  uinta_error_code generate_mesh(std::span<f32> vtx_buffer, std::span<u32> idx_buffer) noexcept override;

  const glm::vec3& color() const noexcept {
    return m_color;
  }

  void color(const glm::vec3& c) noexcept {
    m_color = c;
  }

  virtual u32 vertex_count() const noexcept override {
    return VertexCount;
  }

  virtual u32 index_count() const noexcept override {
    return IndexCount;
  }

  virtual void render(const UiRenderer& renderer) const noexcept override {
  }

 private:
  static constexpr u32 VertexCount = 4;
  static constexpr u32 IndexCount = 6;

  glm::vec3 m_color = {1, 1, 1};
};

}  // namespace uinta

#endif  // UINTA_UI_PANE_HPP
