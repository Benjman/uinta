#include <gtest/gtest.h>
#include <math.hpp>

TEST(vector, vec2) {
    ASSERT_EQ(vec2(0.0, 0.0), vec2()) << "vec2 initial values incorrect.";
    ASSERT_EQ(vec2(3.0, 3.0), vec2(3.0)) << "vec2 single value constructor failed to set values correctly.";

    vec2 vec(1.0, 2.0);
    ASSERT_EQ(vec2(1.0, 2.0), vec2(vec)) << "vec2 copy constructor failed.";

    vec.x = 3.0;
    vec.y = 5.0;
    vec2 moved = vec;
    ASSERT_EQ(vec2(3.0, 5.0), moved) << "vec2 move constructor failed.";

    vec2 assigned = vec2(7.0, 11.0);
    ASSERT_EQ(vec2(7.0, 11.0), assigned) << "vec2 assignment constructor failed.";

    vec2 summed = assigned + vec2(1.0, 2.0);
    ASSERT_EQ(vec2(assigned.x + 1.0, assigned.y + 2.0), summed) << "vec2 sum operator overload failed.";

    vec2 diff = assigned - vec2(1.0, 2.0);
    ASSERT_EQ(vec2(assigned.x - 1.0, assigned.y - 2.0), diff) << "vec2 difference operator overload failed.";

    ASSERT_TRUE(vec2(1.0, 1.0) == vec2(1.0, 1.0)) << "vec2 equality operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 1.0) == vec2(1.0, 0.0)) << "vec2 equality operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 1.0) == vec2(0.0, 1.0)) << "vec2 equality operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 1.0) == vec2(0.0, 0.0)) << "vec2 equality operator overload failed.";

    ASSERT_TRUE(vec2(1.0, 1.0) > vec2(0.0, 0.0)) << "vec2 greater than operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 0.0) > vec2(0.0, 0.0)) << "vec2 greater than operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 1.0) > vec2(0.0, 0.0)) << "vec2 greater than operator overload failed.";

    ASSERT_TRUE(vec2(1.0, 1.0) >= vec2(0.0, 0.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec2(1.0, 0.0) >= vec2(0.0, 0.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec2(0.0, 1.0) >= vec2(0.0, 0.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 1.0) >= vec2(2.0, 0.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 0.0) >= vec2(2.0, 0.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 1.0) >= vec2(2.0, 0.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 1.0) >= vec2(0.0, 2.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 0.0) >= vec2(0.0, 2.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 1.0) >= vec2(0.0, 2.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 1.0) >= vec2(2.0, 2.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(1.0, 0.0) >= vec2(2.0, 2.0)) << "vec2 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 1.0) >= vec2(2.0, 2.0)) << "vec2 greater than or equal to operator overload failed.";

    ASSERT_TRUE(vec2(0.0, 0.0) < vec2(1.0, 1.0)) << "vec2 less than operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 0.0) < vec2(1.0, 0.0)) << "vec2 less than operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 0.0) < vec2(0.0, 1.0)) << "vec2 less than operator overload failed.";

    ASSERT_TRUE(vec2(0.0, 0.0) <= vec2(1.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_TRUE(vec2(0.0, 0.0) <= vec2(1.0, 0.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_TRUE(vec2(0.0, 0.0) <= vec2(0.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(2.0, 0.0) <= vec2(1.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(2.0, 0.0) <= vec2(1.0, 0.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(2.0, 0.0) <= vec2(0.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 2.0) <= vec2(1.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 2.0) <= vec2(1.0, 0.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(0.0, 2.0) <= vec2(0.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(2.0, 2.0) <= vec2(1.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(2.0, 2.0) <= vec2(1.0, 0.0)) << "vec2 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec2(2.0, 2.0) <= vec2(0.0, 1.0)) << "vec2 less than or equal to operator overload failed.";
}

TEST(vector, vec3) {
    ASSERT_EQ(vec3(0.0, 0.0, 0.0), vec3()) << "vec3 initial values incorrect.";
    ASSERT_EQ(vec3(3.0, 3.0, 3.0), vec3(3.0)) << "vec3 single value constructor failed to set values correctly.";

    vec3 vec(1.0, 2.0, 3.0);
    vec3 copied(vec);
    ASSERT_EQ(vec3(1.0, 2.0, 3.0), copied) << "vec3 copy constructor failed.";

    copied.x = 5.0;
    copied.y = 7.0;
    copied.z = 11.0;
    vec3 moved = copied;
    ASSERT_EQ(vec3(5.0, 7.0, 11.0), moved) << "vec3 copy constructor failed.";

    vec3 assigned = vec3(13.0, 17.0, 23.0);
    ASSERT_EQ(vec3(13.0, 17.0, 23.0), assigned) << "vec3 move constructor failed.";

    ASSERT_TRUE(vec3(1.0, 1.0, 1.0) == vec3(1.0, 1.0, 1.0)) << "vec3 equality operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) == vec3(1.0, 0.0, 0.0)) << "vec3 equality operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) == vec3(0.0, 1.0, 0.0)) << "vec3 equality operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) == vec3(0.0, 0.0, 1.0)) << "vec3 equality operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) == vec3(0.0, 0.0, 0.0)) << "vec3 equality operator overload failed.";

    ASSERT_TRUE(vec3(1.0, 1.0, 1.0) > vec3(0.0, 0.0, 0.0)) << "vec3 greater than operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 0.0, 0.0) > vec3(0.0, 0.0, 0.0)) << "vec3 greater than operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 1.0, 0.0) > vec3(0.0, 0.0, 0.0)) << "vec3 greater than operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 1.0) > vec3(0.0, 0.0, 0.0)) << "vec3 greater than operator overload failed.";

    ASSERT_TRUE(vec3(1.0, 1.0, 1.0) >= vec3(0.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec3(1.0, 0.0, 0.0) >= vec3(0.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec3(0.0, 1.0, 0.0) >= vec3(0.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_TRUE(vec3(0.0, 0.0, 1.0) >= vec3(0.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) >= vec3(2.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 0.0, 0.0) >= vec3(2.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 1.0, 0.0) >= vec3(2.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 1.0) >= vec3(2.0, 0.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) >= vec3(0.0, 2.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 0.0, 0.0) >= vec3(0.0, 2.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 1.0, 0.0) >= vec3(0.0, 2.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 1.0) >= vec3(0.0, 2.0, 0.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) >= vec3(0.0, 0.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 0.0, 0.0) >= vec3(0.0, 0.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 1.0, 0.0) >= vec3(0.0, 0.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 1.0) >= vec3(0.0, 0.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 1.0, 1.0) >= vec3(2.0, 2.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(1.0, 0.0, 0.0) >= vec3(2.0, 2.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 1.0, 0.0) >= vec3(2.0, 2.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 1.0) >= vec3(2.0, 2.0, 2.0)) << "vec3 greater than or equal to operator overload failed.";

    ASSERT_TRUE(vec3(0.0, 0.0, 0.0) < vec3(1.0, 1.0, 1.0)) << "vec3 less than operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 0.0) < vec3(1.0, 0.0, 0.0)) << "vec3 less than operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 0.0) < vec3(0.0, 1.0, 0.0)) << "vec3 less than operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 0.0) < vec3(0.0, 0.0, 1.0)) << "vec3 less than operator overload failed.";
    ASSERT_TRUE(vec3(0.0, 0.0, 0.0) <= vec3(1.0, 1.0, 1.0)) << "vec3 less than or equal to operator overload failed.";

    ASSERT_TRUE(vec3(0.0, 0.0, 0.0) <= vec3(1.0, 0.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_TRUE(vec3(0.0, 0.0, 0.0) <= vec3(0.0, 1.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_TRUE(vec3(0.0, 0.0, 0.0) <= vec3(0.0, 0.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 0.0, 0.0) <= vec3(1.0, 1.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 0.0, 0.0) <= vec3(1.0, 0.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 0.0, 0.0) <= vec3(0.0, 1.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 0.0, 0.0) <= vec3(0.0, 0.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 2.0, 0.0) <= vec3(1.0, 1.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 2.0, 0.0) <= vec3(1.0, 0.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 2.0, 0.0) <= vec3(0.0, 1.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 2.0, 0.0) <= vec3(0.0, 0.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 2.0) <= vec3(1.0, 1.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 2.0) <= vec3(1.0, 0.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 2.0) <= vec3(0.0, 1.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(0.0, 0.0, 2.0) <= vec3(0.0, 0.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 2.0, 2.0) <= vec3(1.0, 1.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 2.0, 2.0) <= vec3(1.0, 0.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 2.0, 2.0) <= vec3(0.0, 1.0, 0.0)) << "vec3 less than or equal to operator overload failed.";
    ASSERT_FALSE(vec3(2.0, 2.0, 2.0) <= vec3(0.0, 0.0, 1.0)) << "vec3 less than or equal to operator overload failed.";
}
