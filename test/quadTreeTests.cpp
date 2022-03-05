#include <gtest/gtest.h>
#include <quadTree.h>

TEST(quadTree, bounds) {
    // quad q;
    // ASSERT_EQ(vec2d(0, 0), q.q);
    // ASSERT_EQ(vec2d(0, 0), q.bottomRightBounds);
    //
    // quad withBounds(vec2d(15.0, 15.0), vec2d(30.0, 30.0));
    // ASSERT_EQ(vec2d(15.0, 15.0), q.q);
    // ASSERT_EQ(vec2d(0, 0), q.bottomRightBounds);
}

TEST(quadTree, isInBounds) {
    quad q(vec2d(0.0, 0.0), vec2d(1.0, 1.0));
    ASSERT_TRUE(q.isInBounds(vec2d(.5, .5))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2d(1.0, 1.0))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2d(0.6, 0.6))) << "quad.isInBounds check failed.";
    ASSERT_FALSE(q.isInBounds(vec2d(1.1, 0.6))) << "quad.isInBounds greater than x-axis check failed.";
    ASSERT_FALSE(q.isInBounds(vec2d(0.6, 1.1))) << "quad.isInBounds greater than y-axis check failed.";
    ASSERT_FALSE(q.isInBounds(vec2d(1.1, 1.1))) << "quad.isInBounds greater than both axes check failed.";
    ASSERT_FALSE(q.isInBounds(vec2d(-0.1, 0.1))) << "quad.isInBounds less than x-axis check failed.";
    ASSERT_FALSE(q.isInBounds(vec2d(6.0, 4.0))) << "quad.isInBounds less than y-axis check failed.";
    ASSERT_FALSE(q.isInBounds(vec2d(4.0, 4.0))) << "quad.isInBounds less than both axes check failed.";
}

