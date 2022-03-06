#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include <entt/entt.hpp>

#define private public
#include <quadtree.h>

TEST(quadtree, constructorValidations_spatialAlignment) {
    bool thrown = false;

    try {
        quad(vec2(8.0), vec2(0.0, 16));
    } catch (const std::runtime_error e) {
        thrown = true;
        EXPECT_STREQ("bottomRightBounds must be aligned after the topRightBounds spatially.", e.what());
    }
    ASSERT_TRUE(thrown) << "bounds alignment validation not executed";

    quad q;
    vec3 v;
    std::cout << sizeof(v) << std::endl;

    try {
        quad(vec2(8.0), vec2(18.0, 2.0));
    } catch (const std::runtime_error e) {
        thrown = true;
        EXPECT_STREQ("bottomRightBounds must be aligned after the topRightBounds spatially.", e.what());
        return;
    }

    FAIL() << "validation not executed -- quad boundaries must be a square -- validation not run";
}

TEST(quadtree, constructorValidations_boundsArePowersOf2) {
    bool thrown = false;

    try {
        quad(vec2(), vec2(8, 16));
    } catch (const std::runtime_error e) {
        thrown = true;
        EXPECT_STREQ("quad boundaries must be a square", e.what());
        return;
    }
    FAIL() << "validation not executed -- quad boundaries must be a square -- validation not run";
}

TEST(quadtree, constructorValidations_boundsAreSquares) {
    bool thrown = false;

    try {
        quad(vec2(), vec2(8, 16));
    } catch (const std::runtime_error e) {
        thrown = true;
        EXPECT_STREQ("quad boundaries must be a square", e.what());
        return;
    }
    FAIL() << "validation not executed -- quad boundaries must be a square -- validation not run";
}

TEST(quadtree, constructorValidations_minBoundarySize) {
    bool thrown = false;

    try {
        quad(vec2(0.0, 0.0), vec2(5.0, 10.0), 10);
    } catch (const std::runtime_error e) {
        thrown = true;
        EXPECT_STREQ("quad width must be at least the size of the minimum cell size", e.what());
    }
    ASSERT_TRUE(thrown) << "quad size width validation not executed";

    try {
        quad(vec2(0.0, 0.0), vec2(10.0, 5.0), 10);
    } catch (const std::runtime_error e) {
        EXPECT_STREQ("quad height must be at least the size of the minimum cell size", e.what());
        return;
    }
    FAIL() << "validation not executed -- quad boundaries must be a square -- validation not run";
}

TEST(quadtree, constructorValidations_wholeNumberedBounds) {
    // The reason this test is commented out is because all validations are in one method, so we can't validate everything.
    // Uncomment this test if we isolate the validations.


    // bool thrown = false;
    //
    // try {
    //     quad(vec2(0.5, 1.0), vec2(1.0));
    // } catch (const std::runtime_error e) {
    //     thrown = true;
    //     EXPECT_STREQ("quad.topLeftBounds.x must be an integer.", e.what());
    // }
    // ASSERT_TRUE(thrown) << "quad.topLeftBounds.x must be an integer exception not thrown.";
    // thrown = false;
    //
    // try {
    //     quad(vec2(1.0, 0.5), vec2(1.0));
    // } catch (const std::runtime_error e) {
    //     thrown = true;
    //     EXPECT_STREQ("quad.topLeftBounds.y must be an integer.", e.what());
    // }
    // ASSERT_TRUE(thrown) << "quad.topLeftBounds.y must be an integer exception not thrown.";
    // thrown = false;
    //
    // try {
    //     quad(vec2(1.0), vec2(0.5, 1.0));
    // } catch (const std::runtime_error e) {
    //     thrown = true;
    //     EXPECT_STREQ("quad.bottomRightBounds.x must be an integer.", e.what());
    // }
    // ASSERT_TRUE(thrown) << "quad.bottomRightBounds.x must be an integer exception not thrown.";
    // thrown = false;
    //
    // try {
    //     quad(vec2(1.0), vec2(1.0, 0.5));
    // } catch (const std::runtime_error e) {
    //     thrown = true;
    //     EXPECT_STREQ("quad.bottomRightBounds.y must be an integer.", e.what());
    // }
    // ASSERT_TRUE(thrown) << "quad.bottomRightBounds.y must be an integer exception not thrown.";
    // thrown = false;
    //
    // quad(VALID_TOP_LEFT, VALID_BOTTOM_RIGHT);
}

TEST(quadtree, isInBounds) {
    quad q(vec2(2.0), vec2(10.0));
    ASSERT_TRUE(q.isInBounds(vec2(2.0))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(2.0, 10.0))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(10.0))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(10.0, 2.0))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(7.0))) << "quad.isInBounds check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(2.1, 10.0))) << "quad.isInBounds inside greater than x-axis check failed.";

    ASSERT_TRUE(q.isInBounds(vec2(2.1))) << "quad.isInBounds inside top-left check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(2.1, 9.9))) << "quad.isInBounds inside bottom-left check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(9.9))) << "quad.isInBounds inside bottom-right check failed.";
    ASSERT_TRUE(q.isInBounds(vec2(9.9, 2.1))) << "quad.isInBounds inside top-right check failed.";

    ASSERT_FALSE(q.isInBounds(vec2(1.9, 2.0))) << "quad.isInBounds outside top-left horizontal check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(2.0, 1.9))) << "quad.isInBounds outside top-left vertical check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(10.0, 1.9))) << "quad.isInBounds outside top-right vertical check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(10.1, 2.0))) << "quad.isInBounds outside top-right horizontal check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(10.1, 10.0))) << "quad.isInBounds outside bottom-right horizontal check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(10.0, 10.1))) << "quad.isInBounds outside bottom-right vertical check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(2.0, 10.1))) << "quad.isInBounds outside bottom-left vertical check failed.";
    ASSERT_FALSE(q.isInBounds(vec2(1.9, 10.0))) << "quad.isInBounds outside bottom-left horizontal check failed.";
}

TEST(quadtree, initialState) {
    ASSERT_EQ(2.0, quad::MIN_CELL_SIZE) << "Default minimum cell size should be 2 to accommodate power of 2 rule.";

    quad noParams;
    ASSERT_EQ(nullptr, noParams.bottomRight) << "quad.bottomRight should be initialized to null.";
    ASSERT_EQ(nullptr, noParams.bottomLeft) << "quad.bottomLeft should be initialized to null.";
    ASSERT_EQ(nullptr, noParams.topRight) << "quad.topRight should be initialized to null.";
    ASSERT_EQ(nullptr, noParams.topLeft) << "quad.topLeft should be initialized to null.";
    ASSERT_EQ(vec2(0.0), noParams.topLeftBounds) << "quad.topRightBounds should initialize to vec2(0.0).";
    ASSERT_EQ(vec2(quad::MIN_CELL_SIZE), noParams.bottomRightBounds)  << "quad.topRightBounds should initialize to vec2(quad::MIN_CELL_SIZE).";
    ASSERT_EQ(nullptr, noParams.entityStore) << "quad.topLeft should be initialized to null.";
    ASSERT_EQ(0, noParams.entityCount) << "quad.entityCount should initialize to 0.";

    quad withBounds(vec2(0), vec2(2));
    ASSERT_EQ(nullptr, withBounds.bottomRight) << "quad.bottomRight should be initialized to null.";
    ASSERT_EQ(nullptr, withBounds.bottomLeft) << "quad.bottomLeft should be initialized to null.";
    ASSERT_EQ(nullptr, withBounds.topRight) << "quad.topRight should be initialized to null.";
    ASSERT_EQ(nullptr, withBounds.topLeft) << "quad.topLeft should be initialized to null.";
    ASSERT_EQ(0, withBounds.entityCount) << "quad.entityCount should initialize to 0.";
}

TEST(quadtree, addEntity) {
    entt::registry registry;
    entt::entity entity = registry.create();

    quad q;
    q.addEntity(entity);
    ASSERT_EQ(q.entityStore[0], entity);
    ASSERT_EQ(1, q.entityCount);
    ASSERT_EQ(quad::ENTITY_STORE_SIZE_STEP, q.entityStoreSize);

    entt::entity entity2 = registry.create();
    q.addEntity(entity2);
    ASSERT_EQ(q.entityStore[0], entity);
    ASSERT_EQ(q.entityStore[1], entity2);
    ASSERT_EQ(2, q.entityCount);

    while (q.entityCount + 1 != q.entityStoreSize) {
        q.addEntity(registry.create());
    }

    ASSERT_EQ(quad::ENTITY_STORE_SIZE_STEP * 2, q.entityStoreSize);
}

TEST(quadtree, removeEntity) {
    entt::registry registry;
    entt::entity entities[10];

    quad q;
    for (int i = 0; i < 10; i++) {
        entities[i] = registry.create();
        q.addEntity(entities[i]);
    }

    ASSERT_EQ(0, memcmp(entities, q.entityStore, sizeof(entities))) << "Entity storage not as expected.";
    ASSERT_EQ(entities[3], q.entityStore[3]);

    q.removeEntity(entities[3]);

    ASSERT_EQ(9, q.entityCount);
    ASSERT_NE(entities[3], q.entityStore[3]) << "quad element not deleted from store.";
    ASSERT_EQ(entities[9], q.entityStore[3]) << "last element of store should be moved to deleted's spot";
}
