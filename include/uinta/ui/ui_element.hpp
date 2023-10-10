#ifndef UINTA_UI_ELEMENT_HPP
#define UINTA_UI_ELEMENT_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <queue>
#include <span>
#include <string>
#include <uinta/fwd.hpp>
#include <uinta/ui/ui_component.hpp>
#include <uinta/ui/ui_renderer.hpp>
#include <vector>

namespace uinta {

struct Anchor {
  enum class Horizontal : u8 {
    Left,
    Center,
    Right,
  };

  enum class Vertical : u8 {
    Top,
    Middle,
    Bottom,
  };

  Anchor(Horizontal h = Horizontal::Left, Vertical v = Vertical::Top) : horizontal(h), vertical(v) {
  }

  Horizontal horizontal = Horizontal::Left;
  Vertical vertical = Vertical::Top;

  bool operator==(const Anchor& other) const noexcept {
    return other.vertical == vertical && other.horizontal == horizontal;
  }
};

inline std::string to_string(Anchor::Horizontal anchor) {
  switch (anchor) {
    case (Anchor::Horizontal::Left):
      return "Left";
    case (Anchor::Horizontal::Center):
      return "Center";
    case (Anchor::Horizontal::Right):
      return "Right";
    default:
      return "Unknown";
  }
}

inline std::string to_string(Anchor::Vertical anchor) {
  switch (anchor) {
    case (Anchor::Vertical::Top):
      return "Top";
    case (Anchor::Vertical::Middle):
      return "Middle";
    case (Anchor::Vertical::Bottom):
      return "Bottom";
    default:
      return "Unknown";
  }
}

class UiElement {
 public:
  static constexpr u8 ElementsPerVertex = 6;  // position, color

  UiElement(const glm::vec2& position = {0, 0}, const glm::vec2& size = {0, 0}, const Anchor anchor = Anchor()) noexcept
      : m_top_left(glm::vec3(find_min(position, size, anchor), 0)),
        m_bottom_right(glm::vec3(find_max(position, size, anchor), 0)),
        m_anchor(anchor) {
    assert(width() >= 0 && "Unexpected width.");
    assert(height() >= 0 && "Unexpected height.");
  }

  virtual uinta_error_code init();

  virtual void tick(f32 delta) {
    if (!m_component_queue.empty()) {
      auto& component_list = m_component_queue.front();
      for (auto& component : component_list) {
        component->tick(delta);
        if (UiComponent::Status::Completed == component->status()) {
          m_component_queue.pop();
          break;
        }
      }
    }
    for (auto& child : m_children) child->tick(delta);
  }

  virtual bool in_bounds(const glm::vec3& point) const noexcept {
    return point.x >= m_top_left.x && point.y >= m_top_left.y && point.x <= m_bottom_right.x && point.y <= m_bottom_right.y;
  }

  glm::vec3 position() const noexcept {
    return m_top_left;
  }

  glm::vec3 size() const noexcept {
    return m_bottom_right - m_top_left;
  }

  f32 height() const noexcept {
    return m_bottom_right.y - m_top_left.y;
  }

  f32 width() const noexcept {
    return m_bottom_right.x - m_top_left.x;
  }

  Anchor anchor() const noexcept {
    return m_anchor;
  }

  void anchor(Anchor anchor) noexcept {
    m_anchor = anchor;
  }

  void queue_components(std::vector<std::unique_ptr<UiComponent>> components) {
    m_component_queue.push(std::move(components));
  }

  void add_child(std::unique_ptr<UiElement> child) noexcept {
    assert(child && "Invalid child.");
    child.get()->m_parent = this;
    m_children.emplace_back(std::move(child));
  }

  std::span<const std::unique_ptr<UiElement>> children() const noexcept {
    return m_children;
  }

  virtual u32 vertex_count() const noexcept = 0;

  virtual u32 index_count() const noexcept = 0;

  virtual void render(const UiRenderer& renderer) const noexcept = 0;

  virtual uinta_error_code generate_mesh(std::span<f32> vtx_buffer, std::span<u32> idx_buffer) noexcept = 0;

  /**
   * @brief Two elements defining `vertex_count` and `index_count`, in that order.
   */
  virtual std::array<u32, 2> mesh_info() const noexcept {
    return {vertex_count(), index_count()};
  }

 protected:
  std::queue<std::vector<std::unique_ptr<UiComponent>>> m_component_queue;
  std::vector<std::unique_ptr<UiElement>> m_children;
  glm::vec3 m_top_left;
  glm::vec3 m_bottom_right;
  UiElement* m_parent = nullptr;
  Anchor m_anchor;

 private:
  static constexpr glm::vec2 find_min(const glm::vec2& position, const glm::vec2& size, const Anchor& anchor) {
    auto result = position;
    if (Anchor::Horizontal::Center == anchor.horizontal) result.x -= size.x * 0.5;
    if (Anchor::Vertical::Middle == anchor.vertical) result.y -= size.y * 0.5;
    return result;
  }

  static constexpr glm::vec2 find_max(const glm::vec2& position, const glm::vec2& size, const Anchor& anchor) {
    auto result = position + size;
    if (Anchor::Horizontal::Center == anchor.horizontal) result.x -= size.x * 0.5;
    if (Anchor::Vertical::Middle == anchor.vertical) result.y -= size.y * 0.5;
    return result;
  }
};

class UiElementBuilder {
 public:
  UiElementBuilder(const glm::vec2& pos, const glm::vec2& size = {0, 0}, Anchor anchor = {})
      : m_pos(pos), m_size(size), m_anchor(anchor) {
  }

  UiElementBuilder& pos(const glm::vec2& pos) noexcept {
    m_pos = pos;
    return *this;
  }

  UiElementBuilder& size(const glm::vec2& size) noexcept {
    m_size = size;
    return *this;
  }

  UiElementBuilder& anchor(Anchor anchor) noexcept {
    m_anchor = anchor;
    return *this;
  }

 private:
  glm::vec2 m_pos;
  glm::vec2 m_size;
  Anchor m_anchor;
};

}  // namespace uinta

#endif  // UINTA_UI_ELEMENT_HPP
