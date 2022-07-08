#include <gtest/gtest.h>

#include <uinta/math/smooth_float.hpp>

using namespace uinta;

TEST(SmoothFloat, initial_with_agility) {
  ASSERT_EQ(5.0, SmoothFloat(2.0, 5.0).agility);
  ASSERT_EQ(2.0, SmoothFloat(2.0, 5.0).current);
  ASSERT_EQ(2.0, SmoothFloat(2.0, 5.0).target);
}

TEST(SmoothFloat, initial_with_agility_and_target) {
  ASSERT_EQ(3.0, SmoothFloat(2.0, 3.0).agility);
  ASSERT_EQ(2.0, SmoothFloat(2.0, 3.0).current);
  ASSERT_EQ(2.0, SmoothFloat(2.0, 3.0).target);
}

TEST(SmoothFloat, copy_constructor) {
  SmoothFloat f(2.0, 3.0);
  SmoothFloat f2(f);
  ASSERT_EQ(3.0, f2.agility);
  ASSERT_EQ(2.0, f2.current);
  ASSERT_EQ(2.0, f2.target);
}

TEST(SmoothFloat, move_operator) {
  SmoothFloat f(2.0, 3.0);
  SmoothFloat f2 = f;
  ASSERT_EQ(3.0, f2.agility);
  ASSERT_EQ(2.0, f2.current);
  ASSERT_EQ(2.0, f2.target);
}

TEST(SmoothFloat, plus_equals_operator) {
  SmoothFloat f(2.0, 3.0);
  ASSERT_EQ(2.0, f.target);
  f += 5.0;
  ASSERT_EQ(7.0, f.target);
}

TEST(SmoothFloat, minus_equals_operator) {
  SmoothFloat f(2.0, 3.0);
  ASSERT_EQ(2.0, f.target);
  f -= 5.0;
  ASSERT_EQ(-3.0, f.target);
}

// TEST(SmoothFloat, float_assignment_operator) {
//   SmoothFloat f(2.0, 3.0);
//   float v = f;
//   ASSERT_EQ(0.0, v);
//   f.current = 20.0;
//   v         = f;
//   ASSERT_EQ(20.0, v);
// }

TEST(SmoothFloat, force) {
  SmoothFloat f(2.0, 3.0);
  ASSERT_EQ(2.0, f.current);
  f.target += 5.0;
  ASSERT_EQ(2.0, f.current);
  f.force();
  ASSERT_EQ(7.0, f.current);
  ASSERT_EQ(7.0, f.target);
}

TEST(SmoothFloat, force_with_val) {
  SmoothFloat f(2.0, 3.0);
  ASSERT_EQ(2.0, f.current);
  f.force(15.0);
  ASSERT_EQ(15.0, f.current);
  ASSERT_EQ(15.0, f.target);
}

TEST(SmoothFloat, tick) {
  float agility = 2.0;
  SmoothFloat f(0, agility);
  ASSERT_EQ(0.0, f.current);
  ASSERT_EQ(0.0, f.target);

  f.target = 5.0;

  float dt = 0.25;
  f.tick(dt);
  ASSERT_EQ(f.target * agility * dt, f.current);
}
