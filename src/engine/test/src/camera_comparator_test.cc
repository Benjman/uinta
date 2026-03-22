#include "uinta/camera/camera_comparator.h"

#include <gtest/gtest.h>

#include "uinta/camera/camera.h"
#include "uinta/camera/camera_input_deltas.h"

namespace uinta {
namespace {

class CameraComparatorTest : public ::testing::Test {
 protected:
  static CameraConfig makeConfig() noexcept {
    CameraConfig config;
    config.angle = SmoothFloat(1000.0f, 0.0f);
    config.pitch = SmoothFloat(1000.0f, 0.0f);
    config.dist = SmoothFloat(1000.0f, 10.0f);
    config.target = SmoothVec3(1000.0f, 0.0f, 0.0f, 0.0f);
    return config;
  }

  CameraInputDeltas deltas_;
};

TEST_F(CameraComparatorTest, DefaultComparatorDoesNotEqualCamera) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, deltas_);

  // Default comparator should not match camera with non-zero position
  // (camera at dist=10 will have non-zero position)
  EXPECT_TRUE(comparator != &camera);
}

TEST_F(CameraComparatorTest, AssignmentFromCamera) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, deltas_);

  comparator = camera;

  // After assignment, should equal
  EXPECT_TRUE(comparator == &camera);
  EXPECT_FALSE(comparator != &camera);
}

TEST_F(CameraComparatorTest, EqualityAfterAssignment) {
  CameraComparator comparator;
  auto config = makeConfig();
  config.angle = SmoothFloat(1000.0f, 45.0f);
  config.pitch = SmoothFloat(1000.0f, 30.0f);
  Camera camera(config);
  camera.force();
  camera.update(0.016, deltas_);

  comparator = camera;

  EXPECT_TRUE(comparator == &camera);
}

TEST_F(CameraComparatorTest, InequalityWhenPositionDiffers) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, deltas_);

  comparator = camera;

  // Modify camera position by changing target
  camera.target(glm::vec3(100.0f, 0.0f, 0.0f));
  camera.force();
  camera.update(0.016, deltas_);

  EXPECT_TRUE(comparator != &camera);
  EXPECT_FALSE(comparator == &camera);
}

TEST_F(CameraComparatorTest, InequalityWhenPitchDiffers) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, deltas_);

  comparator = camera;

  // Modify pitch
  camera.pitch(45.0f);
  camera.force();

  EXPECT_TRUE(comparator != &camera);
}

TEST_F(CameraComparatorTest, InequalityWhenAngleDiffers) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, deltas_);

  comparator = camera;

  // Modify angle
  camera.angle(90.0f);
  camera.force();

  EXPECT_TRUE(comparator != &camera);
}

TEST_F(CameraComparatorTest, EqualityWithinEpsilon) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, deltas_);

  comparator = camera;

  // Very small changes within epsilon should still be equal
  // The comparator uses epsilon_f for comparison
  EXPECT_TRUE(comparator == &camera);
}

TEST_F(CameraComparatorTest, MultipleAssignments) {
  CameraComparator comparator;
  Camera camera1(makeConfig());
  camera1.force();
  camera1.update(0.016, deltas_);

  auto config2 = makeConfig();
  config2.angle = SmoothFloat(1000.0f, 90.0f);
  Camera camera2(config2);
  camera2.force();
  camera2.update(0.016, deltas_);

  comparator = camera1;
  EXPECT_TRUE(comparator == &camera1);
  EXPECT_TRUE(comparator != &camera2);

  comparator = camera2;
  EXPECT_TRUE(comparator == &camera2);
  EXPECT_TRUE(comparator != &camera1);
}

TEST_F(CameraComparatorTest, AssignmentCapturesCurrentValues) {
  CameraComparator comparator;
  Camera camera(makeConfig());
  camera.angle(45.0f);  // Set target but don't force
  camera.update(0.016, deltas_);

  comparator = camera;

  // Comparator should capture current values, not target
  camera.force();  // Now current == target

  // After force, if we hadn't updated comparator, values may differ
  // depending on how much interpolation happened
}

}  // namespace
}  // namespace uinta
