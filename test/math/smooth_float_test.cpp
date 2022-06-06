#include <gtest/gtest.h>

#include <uinta/math.hpp>

TEST(smooth_float, initial) {
  ASSERT_EQ(1.0, smooth_float().agility);
  ASSERT_EQ(0.0, smooth_float().current);
  ASSERT_EQ(0.0, smooth_float().target);
}

TEST(smooth_float, initial_with_agility) {
  ASSERT_EQ(2.0, smooth_float(2.0).agility);
  ASSERT_EQ(0.0, smooth_float(2.0).current);
  ASSERT_EQ(0.0, smooth_float(2.0).target);
}

TEST(smooth_float, initial_with_agility_and_target) {
  ASSERT_EQ(2.0, smooth_float(2.0, 3.0).agility);
  ASSERT_EQ(0.0, smooth_float(2.0, 3.0).current);
  ASSERT_EQ(3.0, smooth_float(2.0, 3.0).target);
}

TEST(smooth_float, copy_constructor) {
  smooth_float f(2.0, 3.0);
  smooth_float f2(f);
  ASSERT_EQ(2.0, f2.agility);
  ASSERT_EQ(0.0, f2.current);
  ASSERT_EQ(3.0, f2.target);
}

TEST(smooth_float, move_operator) {
  smooth_float f(2.0, 3.0);
  smooth_float f2 = f;
  ASSERT_EQ(2.0, f2.agility);
  ASSERT_EQ(0.0, f2.current);
  ASSERT_EQ(3.0, f2.target);
}

TEST(smooth_float, plus_equals_operator) {
  smooth_float f(2.0, 3.0);
  ASSERT_EQ(3.0, f.target);
  f += 5.0;
  ASSERT_EQ(8.0, f.target);
}

TEST(smooth_float, minus_equals_operator) {
  smooth_float f(2.0, 3.0);
  ASSERT_EQ(3.0, f.target);
  f -= 5.0;
  ASSERT_EQ(-2.0, f.target);
}

TEST(smooth_float, float_assignment_operator) {
  smooth_float f(2.0, 3.0);
  float v = f;
  ASSERT_EQ(0.0, v);
  f.current = 20.0;
  v         = f;
  ASSERT_EQ(20.0, v);
}

TEST(smooth_float, force) {
  smooth_float f(2.0, 3.0);
  ASSERT_EQ(0.0, f.current);
  f.force();
  ASSERT_EQ(3.0, f.current);
  ASSERT_EQ(3.0, f.target);
}

TEST(smooth_float, force_with_val) {
  smooth_float f(2.0, 3.0);
  ASSERT_EQ(0.0, f.current);
  f.force(15.0);
  ASSERT_EQ(15.0, f.current);
  ASSERT_EQ(15.0, f.target);
}

TEST(smooth_float, update) {
  float agility = 2.0;
  float target  = 3.0;
  float dt      = 0.25;
  smooth_float f(agility, target);
  ASSERT_EQ(0.0, f.current);
  f.update(dt);
  ASSERT_EQ((target)*agility * dt, f.current);
}
