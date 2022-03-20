#include <cstring>
#include <gtest/gtest.h>
#include <math.hpp>

TEST(mat4, identity) {
    mat4 m;
    ASSERT_EQ(1.0, m.m00());
    ASSERT_EQ(0.0, m.m01());
    ASSERT_EQ(0.0, m.m02());
    ASSERT_EQ(0.0, m.m03());
    ASSERT_EQ(0.0, m.m10());
    ASSERT_EQ(1.0, m.m11());
    ASSERT_EQ(0.0, m.m12());
    ASSERT_EQ(0.0, m.m13());
    ASSERT_EQ(0.0, m.m20());
    ASSERT_EQ(0.0, m.m21());
    ASSERT_EQ(1.0, m.m22());
    ASSERT_EQ(0.0, m.m23());
    ASSERT_EQ(0.0, m.m30());
    ASSERT_EQ(0.0, m.m31());
    ASSERT_EQ(0.0, m.m32());
    ASSERT_EQ(1.0, m.m33());
}

TEST(mat4, identity_scalar) {
    mat4 m(2);
    ASSERT_EQ(2.0, m.m00());
    ASSERT_EQ(0.0, m.m01());
    ASSERT_EQ(0.0, m.m02());
    ASSERT_EQ(0.0, m.m03());
    ASSERT_EQ(0.0, m.m10());
    ASSERT_EQ(2.0, m.m11());
    ASSERT_EQ(0.0, m.m12());
    ASSERT_EQ(0.0, m.m13());
    ASSERT_EQ(0.0, m.m20());
    ASSERT_EQ(0.0, m.m21());
    ASSERT_EQ(2.0, m.m22());
    ASSERT_EQ(0.0, m.m23());
    ASSERT_EQ(0.0, m.m30());
    ASSERT_EQ(0.0, m.m31());
    ASSERT_EQ(0.0, m.m32());
    ASSERT_EQ(2.0, m.m33());
}

TEST(mat4, constructor_move) {
    mat4 m;
    m.values[0]  = 1.0;
    m.values[1]  = 2.0;
    m.values[2]  = 3.0;
    m.values[3]  = 4.0;
    m.values[4]  = 5.0;
    m.values[5]  = 6.0;
    m.values[6]  = 7.0;
    m.values[7]  = 8.0;
    m.values[8]  = 9.0;
    m.values[9]  = 10.0;
    m.values[10] = 11.0;
    m.values[11] = 12.0;
    m.values[12] = 13.0;
    m.values[13] = 14.0;
    m.values[14] = 15.0;
    m.values[15] = 16.0;
    m = mat4();

    ASSERT_EQ(1.0, m.m00());
    ASSERT_EQ(2.0, m.m00());
    ASSERT_EQ(3.0, m.m00());
    ASSERT_EQ(4.0, m.m00());
    ASSERT_EQ(5.0, m.m00());
    ASSERT_EQ(6.0, m.m00());
    ASSERT_EQ(7.0, m.m00());
    ASSERT_EQ(8.0, m.m00());
    ASSERT_EQ(9.0, m.m00());
    ASSERT_EQ(10.0, m.m00());
    ASSERT_EQ(11.0, m.m00());
    ASSERT_EQ(12.0, m.m00());
    ASSERT_EQ(13.0, m.m00());
    ASSERT_EQ(14.0, m.m00());
    ASSERT_EQ(15.0, m.m00());
    ASSERT_EQ(16.0, m.m00());

}
