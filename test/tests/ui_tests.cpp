#include <gtest/gtest.h>

#define private public
#define protected public
#include <uinta/ui/ui_element.hpp>

using namespace uinta;

class MockUiRenderer : public UiRenderer {
 public:
  virtual RenderState render() noexcept override = 0;
};

class TestUiElement : public UiElement {
 public:
  TestUiElement(const glm::vec3& position = {0, 0, 0}, const glm::vec3& size = {0, 0, 0}, const Anchor anchor = Anchor()) noexcept
      : UiElement(position, size, anchor) {
  }

  void generate_mesh(std::span<f32> vtx_buffer, std::span<u32> idx_buffer) const noexcept override {
  }

  void render(const UiRenderer& renderer) const noexcept override {
  }

  constexpr std::array<u32, 2> mesh_info() const noexcept override {
    return {0, 0};
  };

  virtual constexpr u32 vertex_count() const noexcept override {
    return 0;
  }

  virtual constexpr u32 index_count() const noexcept override {
    return 0;
  }
};

TEST(UiElement, initial_state) {
  TestUiElement element({15, 20, 0}, {25, 30, 0});
  ASSERT_EQ(15, element.m_top_left.x) << "Unexpected minimum x-value.";
  ASSERT_EQ(20, element.m_top_left.y) << "Unexpected minimum y-value.";
  ASSERT_EQ(40, element.m_bottom_right.x) << "Unexpected maximum x-value.";
  ASSERT_EQ(50, element.m_bottom_right.y) << "Unexpected maximum y-value.";
  ASSERT_EQ(30, element.height()) << "Unexpected height.";
  ASSERT_EQ(25, element.width()) << "Unexpected width.";
  ASSERT_EQ(Anchor(Anchor::Horizontal::Left, Anchor::Vertical::Top), element.anchor());
}

TEST(UiElement, bad_size) {
  ASSERT_DEATH(TestUiElement element({15, 20, 0}, {0, -1, 0});, "Unexpected height.") << "Expected death for an invalid height.";
  ASSERT_DEATH(TestUiElement element({15, 20, 0}, {-1, 0, 0});, "Unexpected width.") << "Expected death for an invalid width.";
}

TEST(UiElement, position) {
  TestUiElement element({15, 20, 0}, {25, 30, 0});
  ASSERT_EQ(15, element.position().x) << "Unexpected position x-value.";
  ASSERT_EQ(20, element.position().y) << "Unexpected position y-value.";
}

TEST(UiElement, size) {
  TestUiElement element({15, 20, 0}, {25, 30, 0});
  ASSERT_EQ(25, element.size().x) << "Unexpected size x-value.";
  ASSERT_EQ(30, element.size().y) << "Unexpected size y-value.";
}

TEST(UiElement, in_bounds) {
  TestUiElement element({15, 20, 0}, {25, 30, 0});
  ASSERT_FALSE(element.in_bounds({15, 19, 0})) << "Expected out of bounds one unit before the y-start along the x-start line.";
  ASSERT_FALSE(element.in_bounds({14, 20, 0})) << "Expected out of bounds one unit before the x-start along the y-start line.";
  ASSERT_FALSE(element.in_bounds({45, 19, 0})) << "Expected out of bounds one unit before the y-start along the x-start line.";
  ASSERT_FALSE(element.in_bounds({14, 50, 0})) << "Expected out of bounds one unit before the x-start along the y-start line.";
  ASSERT_FALSE(element.in_bounds({15, 51, 0})) << "Expected out of bounds one unit after the y-end along the x-start line.";
  ASSERT_FALSE(element.in_bounds({51, 20, 0})) << "Expected out of bounds one unit after the x-end along the y-start line.";
  ASSERT_FALSE(element.in_bounds({45, 51, 0})) << "Expected out of bounds one unit after the y-end along the x-start line.";
  ASSERT_FALSE(element.in_bounds({51, 50, 0})) << "Expected out of bounds one unit after the x-end along the y-start line.";
  for (int x = element.position().x; x < element.width(); ++x)
    for (int y = element.position().y; y < element.height(); ++y)
      ASSERT_TRUE(element.in_bounds({x, y, 0})) << "Expected to be in-bounds of an element.";
}

TEST(UiElement, Anchoring) {
  TestUiElement top_left({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Left, Anchor::Vertical::Top});
  ASSERT_EQ(15, top_left.m_top_left.x) << "Unexpected horizontal min value for anchor top and left.";
  ASSERT_EQ(20, top_left.m_top_left.y) << "Unexpected vertical min value for anchor top and left.";
  ASSERT_EQ(40, top_left.m_bottom_right.x) << "Unexpected horizontal max value for anchor top and left.";
  ASSERT_EQ(50, top_left.m_bottom_right.y) << "Unexpected vertical max value for anchor top and left.";

  TestUiElement top_center({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Center, Anchor::Vertical::Top});
  ASSERT_EQ(2.5, top_center.m_top_left.x) << "Unexpected horizontal min value for anchor top and center.";
  ASSERT_EQ(20, top_center.m_top_left.y) << "Unexpected vertical min value for anchor top and center.";
  ASSERT_EQ(27.5, top_center.m_bottom_right.x) << "Unexpected horizontal max value for anchor top and center.";
  ASSERT_EQ(50, top_center.m_bottom_right.y) << "Unexpected vertical max value for anchor top and center.";

  TestUiElement top_right({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Right, Anchor::Vertical::Top});
  ASSERT_EQ(15, top_right.m_top_left.x) << "Unexpected horizontal min value for anchor top and right.";
  ASSERT_EQ(20, top_right.m_top_left.y) << "Unexpected vertical min value for anchor top and right.";
  ASSERT_EQ(40, top_right.m_bottom_right.x) << "Unexpected horizontal max value for anchor top and right.";
  ASSERT_EQ(50, top_right.m_bottom_right.y) << "Unexpected vertical max value for anchor top and right.";

  TestUiElement middle_left({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Left, Anchor::Vertical::Middle});
  ASSERT_EQ(15, middle_left.m_top_left.x) << "Unexpected horizontal min value for anchor .";
  ASSERT_EQ(5, middle_left.m_top_left.y) << "Unexpected vertical min value for anchor .";
  ASSERT_EQ(40, middle_left.m_bottom_right.x) << "Unexpected horizontal max value for anchor .";
  ASSERT_EQ(35, middle_left.m_bottom_right.y) << "Unexpected vertical max value for anchor .";

  TestUiElement center_middle({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Center, Anchor::Vertical::Middle});
  ASSERT_EQ(2.5, center_middle.m_top_left.x) << "Unexpected horizontal min value for anchor center and middle.";
  ASSERT_EQ(5, center_middle.m_top_left.y) << "Unexpected vertical min value for anchor center and middle.";
  ASSERT_EQ(27.5, center_middle.m_bottom_right.x) << "Unexpected horizontal max value for anchor center and middle.";
  ASSERT_EQ(35, center_middle.m_bottom_right.y) << "Unexpected vertical max value for anchor center and middle.";

  TestUiElement middle_right({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Right, Anchor::Vertical::Middle});
  ASSERT_EQ(15, middle_left.m_top_left.x) << "Unexpected horizontal min value for anchor .";
  ASSERT_EQ(5, middle_left.m_top_left.y) << "Unexpected vertical min value for anchor .";
  ASSERT_EQ(40, middle_left.m_bottom_right.x) << "Unexpected horizontal max value for anchor .";
  ASSERT_EQ(35, middle_left.m_bottom_right.y) << "Unexpected vertical max value for anchor .";

  TestUiElement bottom_left({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Left, Anchor::Vertical::Bottom});
  ASSERT_EQ(15, bottom_left.m_top_left.x) << "Unexpected horizontal min value for anchor bottom and left.";
  ASSERT_EQ(20, bottom_left.m_top_left.y) << "Unexpected vertical min value for anchor bottom and left.";
  ASSERT_EQ(40, bottom_left.m_bottom_right.x) << "Unexpected horizontal max value for anchor bottom and left.";
  ASSERT_EQ(50, bottom_left.m_bottom_right.y) << "Unexpected vertical max value for anchor bottom and left.";

  TestUiElement bottom_center({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Center, Anchor::Vertical::Bottom});
  ASSERT_EQ(2.5, bottom_center.m_top_left.x) << "Unexpected horizontal min value for anchor bottom and center.";
  ASSERT_EQ(20, bottom_center.m_top_left.y) << "Unexpected vertical min value for anchor bottom and center.";
  ASSERT_EQ(27.5, bottom_center.m_bottom_right.x) << "Unexpected horizontal max value for anchor bottom and center.";
  ASSERT_EQ(50, bottom_center.m_bottom_right.y) << "Unexpected vertical max value for anchor bottom and center.";

  TestUiElement bottom_right({15, 20, 0}, {25, 30, 0}, {Anchor::Horizontal::Right, Anchor::Vertical::Bottom});
  ASSERT_EQ(15, bottom_right.m_top_left.x) << "Unexpected horizontal min value for anchor bottom and right.";
  ASSERT_EQ(20, bottom_right.m_top_left.y) << "Unexpected vertical min value for anchor bottom and right.";
  ASSERT_EQ(40, bottom_right.m_bottom_right.x) << "Unexpected horizontal max value for anchor bottom and right.";
  ASSERT_EQ(50, bottom_right.m_bottom_right.y) << "Unexpected vertical max value for anchor bottom and right.";
}
