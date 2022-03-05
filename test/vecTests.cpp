#include <gtest/gtest.h>
#include <vec.h>

TEST(vector, vec2d) {
    vec2d vec;
    ASSERT_EQ(0.0, vec.x);
    ASSERT_EQ(0.0, vec.y);

    vec.x = 1.0;
    vec.y = 2.0;
    vec2d copied(vec);
    ASSERT_EQ(1.0, copied.x) << "vec2d copy constructor failed.";
    ASSERT_EQ(2.0, copied.y) << "vec2d copy constructor failed.";

    copied.x = 3.0;
    copied.y = 5.0;
    vec2d moved = copied;
    ASSERT_EQ(3.0, moved.x) << "vec2d move constructor failed.";
    ASSERT_EQ(5.0, moved.y) << "vec2d move constructor failed.";

    vec2d assigned = vec2d(7.0, 11.0);
    ASSERT_EQ(7.0, assigned.x) << "vec2d assignment constructor failed.";
    ASSERT_EQ(11.0, assigned.y) << "vec2d assignment constructor failed.";

    vec2d summed = assigned + vec2d(1.0, 2.0);
    ASSERT_EQ(assigned.x + 1.0, summed.x) << "vec2d sum operator overload failed.";
    ASSERT_EQ(assigned.y + 2.0, summed.y) << "vec2d sum operator overload failed.";

    vec2d diff = assigned - vec2d(1.0, 2.0);
    ASSERT_EQ(assigned.x - 1.0, diff.x) << "vec2d difference operator overload failed.";
    ASSERT_EQ(assigned.y - 2.0, diff.y) << "vec2d difference operator overload failed.";

    ASSERT_TRUE(vec2d(1.0, 1.0) == vec2d(1.0, 1.0)) << "vec2d equality operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 1.0) == vec2d(1.0, 0.0)) << "vec2d equality operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 1.0) == vec2d(0.0, 1.0)) << "vec2d equality operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 1.0) == vec2d(0.0, 0.0)) << "vec2d equality operator overload failed.";

    ASSERT_TRUE(vec2d(1.0, 1.0) > vec2d(0.0, 0.0)) << "vec2d greater than operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 0.0) > vec2d(0.0, 0.0)) << "vec2d greater than operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 1.0) > vec2d(0.0, 0.0)) << "vec2d greater than operator overload failed.";

    ASSERT_TRUE(vec2d(1.0, 1.0) >= vec2d(0.0, 0.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec2d(1.0, 0.0) >= vec2d(0.0, 0.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec2d(0.0, 1.0) >= vec2d(0.0, 0.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 1.0) >= vec2d(2.0, 0.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 0.0) >= vec2d(2.0, 0.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 1.0) >= vec2d(2.0, 0.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 1.0) >= vec2d(0.0, 2.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 0.0) >= vec2d(0.0, 2.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 1.0) >= vec2d(0.0, 2.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 1.0) >= vec2d(2.0, 2.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(1.0, 0.0) >= vec2d(2.0, 2.0)) << "vec2d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 1.0) >= vec2d(2.0, 2.0)) << "vec2d greater than or equal to operator overload failed.";

    ASSERT_TRUE(vec2d(0.0, 0.0) < vec2d(1.0, 1.0)) << "vec2d less than operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 0.0) < vec2d(1.0, 0.0)) << "vec2d less than operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 0.0) < vec2d(0.0, 1.0)) << "vec2d less than operator overload failed.";

    ASSERT_TRUE(vec2d(0.0, 0.0) <= vec2d(1.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_TRUE(vec2d(0.0, 0.0) <= vec2d(1.0, 0.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_TRUE(vec2d(0.0, 0.0) <= vec2d(0.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(2.0, 0.0) <= vec2d(1.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(2.0, 0.0) <= vec2d(1.0, 0.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(2.0, 0.0) <= vec2d(0.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 2.0) <= vec2d(1.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 2.0) <= vec2d(1.0, 0.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(0.0, 2.0) <= vec2d(0.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(2.0, 2.0) <= vec2d(1.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(2.0, 2.0) <= vec2d(1.0, 0.0)) << "vec2d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2d(2.0, 2.0) <= vec2d(0.0, 1.0)) << "vec2d less than or equal to operator overload failed.";
}

TEST(vector, vec3d) {
    vec3d vec;
    ASSERT_EQ(0.0, vec.x);
    ASSERT_EQ(0.0, vec.y);
    ASSERT_EQ(0.0, vec.z);

    vec.x = 1.0;
    vec.y = 2.0;
    vec.z = 3.0;
    vec3d copied(vec);
    ASSERT_EQ(1.0, copied.x) << "vec3d copy constructor failed.";
    ASSERT_EQ(2.0, copied.y) << "vec3d copy constructor failed.";
    ASSERT_EQ(3.0, copied.z) << "vec3d copy constructor failed.";

    copied.x = 5.0;
    copied.y = 7.0;
    copied.z = 11.0;
    vec3d moved = copied;
    ASSERT_EQ(5.0, moved.x) << "vec3d move constructor failed.";
    ASSERT_EQ(7.0, moved.y) << "vec3d move constructor failed.";
    ASSERT_EQ(11.0, moved.z) << "vec3d move constructor failed.";

    vec3d assigned = vec3d(13.0, 17.0, 23.0);
    ASSERT_EQ(13.0, assigned.x) << "vec3d move constructor failed.";
    ASSERT_EQ(17.0, assigned.y) << "vec3d move constructor failed.";
    ASSERT_EQ(23.0, assigned.z) << "vec3d move constructor failed.";

    ASSERT_TRUE(vec3d(1.0, 1.0, 1.0) == vec3d(1.0, 1.0, 1.0)) << "vec3d equality operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) == vec3d(1.0, 0.0, 0.0)) << "vec3d equality operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) == vec3d(0.0, 1.0, 0.0)) << "vec3d equality operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) == vec3d(0.0, 0.0, 1.0)) << "vec3d equality operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) == vec3d(0.0, 0.0, 0.0)) << "vec3d equality operator overload failed.";

    ASSERT_TRUE(vec3d(1.0, 1.0, 1.0) > vec3d(0.0, 0.0, 0.0)) << "vec3d greater than operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 0.0, 0.0) > vec3d(0.0, 0.0, 0.0)) << "vec3d greater than operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 1.0, 0.0) > vec3d(0.0, 0.0, 0.0)) << "vec3d greater than operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 1.0) > vec3d(0.0, 0.0, 0.0)) << "vec3d greater than operator overload failed.";

    ASSERT_TRUE(vec3d(1.0, 1.0, 1.0) >= vec3d(0.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec3d(1.0, 0.0, 0.0) >= vec3d(0.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec3d(0.0, 1.0, 0.0) >= vec3d(0.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec3d(0.0, 0.0, 1.0) >= vec3d(0.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) >= vec3d(2.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 0.0, 0.0) >= vec3d(2.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 1.0, 0.0) >= vec3d(2.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 1.0) >= vec3d(2.0, 0.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) >= vec3d(0.0, 2.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 0.0, 0.0) >= vec3d(0.0, 2.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 1.0, 0.0) >= vec3d(0.0, 2.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 1.0) >= vec3d(0.0, 2.0, 0.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) >= vec3d(0.0, 0.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 0.0, 0.0) >= vec3d(0.0, 0.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 1.0, 0.0) >= vec3d(0.0, 0.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 1.0) >= vec3d(0.0, 0.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 1.0, 1.0) >= vec3d(2.0, 2.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(1.0, 0.0, 0.0) >= vec3d(2.0, 2.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 1.0, 0.0) >= vec3d(2.0, 2.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 1.0) >= vec3d(2.0, 2.0, 2.0)) << "vec3d greater than or equal to operator overload failed.";

    ASSERT_TRUE(vec3d(0.0, 0.0, 0.0) < vec3d(1.0, 1.0, 1.0)) << "vec3d less than operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 0.0) < vec3d(1.0, 0.0, 0.0)) << "vec3d less than operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 0.0) < vec3d(0.0, 1.0, 0.0)) << "vec3d less than operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 0.0) < vec3d(0.0, 0.0, 1.0)) << "vec3d less than operator overload failed.";
    ASSERT_TRUE(vec3d(0.0, 0.0, 0.0) <= vec3d(1.0, 1.0, 1.0)) << "vec3d less than or equal to operator overload failed.";

    ASSERT_TRUE(vec3d(0.0, 0.0, 0.0) <= vec3d(1.0, 0.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_TRUE(vec3d(0.0, 0.0, 0.0) <= vec3d(0.0, 1.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_TRUE(vec3d(0.0, 0.0, 0.0) <= vec3d(0.0, 0.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 0.0, 0.0) <= vec3d(1.0, 1.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 0.0, 0.0) <= vec3d(1.0, 0.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 0.0, 0.0) <= vec3d(0.0, 1.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 0.0, 0.0) <= vec3d(0.0, 0.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 2.0, 0.0) <= vec3d(1.0, 1.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 2.0, 0.0) <= vec3d(1.0, 0.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 2.0, 0.0) <= vec3d(0.0, 1.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 2.0, 0.0) <= vec3d(0.0, 0.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 2.0) <= vec3d(1.0, 1.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 2.0) <= vec3d(1.0, 0.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 2.0) <= vec3d(0.0, 1.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(0.0, 0.0, 2.0) <= vec3d(0.0, 0.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 2.0, 2.0) <= vec3d(1.0, 1.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 2.0, 2.0) <= vec3d(1.0, 0.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 2.0, 2.0) <= vec3d(0.0, 1.0, 0.0)) << "vec3d less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3d(2.0, 2.0, 2.0) <= vec3d(0.0, 0.0, 1.0)) << "vec3d less than or equal to operator overload failed.";
}
