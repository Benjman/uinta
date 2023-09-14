#include <gtest/gtest.h>

#include <uinta/math/hexagon.hpp>

namespace uinta {

TEST(Hexagon, rotate) {
  glm::ivec3 vec(1, 0, -1);

  ASSERT_EQ(vec, rotate_cw(vec, 0)) << "No rotation expected.";
  ASSERT_EQ(glm::ivec3(vec.z, vec.x, vec.y), rotate_cw(vec, 2)) << "Unexpected clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.x, -vec.y, -vec.z), rotate_cw(vec, 3)) << "Unexpected clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.y, vec.z, vec.x), rotate_cw(vec, 4)) << "Unexpected clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.z, -vec.x, -vec.y), rotate_cw(vec, 5)) << "Unexpected clockwise rotation.";
  ASSERT_EQ(vec, rotate_cw(vec, 6)) << "Full clockwise rotatation should equate to the original vector.";

  ASSERT_EQ(glm::ivec3(-vec.y, -vec.z, -vec.x), rotate_cw(vec, 7)) << "Unexpected terminal clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.z, vec.x, vec.y), rotate_cw(vec, 8)) << "Unexpected terminal clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.x, -vec.y, -vec.z), rotate_cw(vec, 9)) << "Unexpected terminal clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.y, vec.z, vec.x), rotate_cw(vec, 10)) << "Unexpected terminal clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.z, -vec.x, -vec.y), rotate_cw(vec, 11)) << "Unexpected terminal clockwise rotation.";
  ASSERT_EQ(vec, rotate_cw(vec, 12)) << "Terminal clockwise rotatation should equate to original vector.";

  ASSERT_EQ(glm::ivec3(-vec.z, -vec.x, -vec.y), rotate_ccw(vec, 1)) << "Unexpected counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.y, vec.z, vec.x), rotate_ccw(vec, 2)) << "Unexpected counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.x, -vec.y, -vec.z), rotate_ccw(vec, 3)) << "Unexpected counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.z, vec.x, vec.y), rotate_ccw(vec, 4)) << "Unexpected counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.y, -vec.z, -vec.x), rotate_ccw(vec, 5)) << "Unexpected counter-clockwise rotation.";
  ASSERT_EQ(vec, rotate_ccw(vec, 6)) << "Full counter-clockwise rotatation should equate to original vector.";

  ASSERT_EQ(glm::ivec3(-vec.z, -vec.x, -vec.y), rotate_ccw(vec, 7)) << "Unexpected terminal counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.y, vec.z, vec.x), rotate_ccw(vec, 8)) << "Unexpected terminal counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.x, -vec.y, -vec.z), rotate_ccw(vec, 9)) << "Unexpected terminal counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(vec.z, vec.x, vec.y), rotate_ccw(vec, 10)) << "Unexpected terminal counter-clockwise rotation.";
  ASSERT_EQ(glm::ivec3(-vec.y, -vec.z, -vec.x), rotate_ccw(vec, 11)) << "Unexpected terminal counter-clockwise rotation.";
  ASSERT_EQ(vec, rotate_ccw(vec, 12)) << "Full terminal counter-clockwise rotatation should equate to the original vector.";
}

}  // namespace uinta
