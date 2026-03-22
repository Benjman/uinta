#include "uinta/camera/camera_events.h"

#include <gtest/gtest.h>

#include <glm/ext/matrix_float4x4.hpp>

#include "uinta/camera/camera.h"
#include "uinta/camera/camera_config.h"
#include "uinta/camera/camera_input_deltas.h"

namespace uinta {

class CameraEventsTest : public ::testing::Test {
 protected:
  static CameraConfig makeConfig() noexcept {
    CameraConfig config;
    config.angle = SmoothFloat(1000.0f, 0.0f);
    config.pitch = SmoothFloat(1000.0f, 0.0f);
    config.dist = SmoothFloat(1000.0f, 10.0f);
    config.target = SmoothVec3(1000.0f, 0.0f, 0.0f, 0.0f);
    return config;
  }
};

TEST_F(CameraEventsTest, GetCameraEventForViewMatrixUpdate) {
  auto event = getCameraEvent<ViewMatrixUpdateEvent>();
  EXPECT_EQ(CameraEvent::ViewMatrixUpdated, event);
}

// ViewMatrixUpdateEvent tests
TEST_F(CameraEventsTest, ViewMatrixUpdateEventConstruction) {
  Camera camera(makeConfig());
  glm::mat4 viewMatrix(1.0f);
  ViewMatrixUpdateEvent event{.camera = &camera, .view = viewMatrix};

  EXPECT_EQ(&camera, event.camera);
  EXPECT_EQ(viewMatrix, event.view);
}

// CameraEventPolicies tests
TEST_F(CameraEventsTest, CameraEventPoliciesGetEventView) {
  Camera camera(makeConfig());
  ViewMatrixUpdateEvent event{.camera = &camera, .view = glm::mat4(1.0f)};

  auto cameraEvent = CameraEventPolicies::getEvent(event);
  EXPECT_EQ(CameraEvent::ViewMatrixUpdated, cameraEvent);
}

// CameraDispatchers tests
TEST_F(CameraEventsTest, CameraDispatchersAddAndDispatchViewMatrixUpdate) {
  CameraDispatchers dispatchers;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  bool callbackInvoked = false;
  const Camera* receivedCamera = nullptr;
  glm::mat4 receivedMatrix;

  dispatchers.addListener<CameraEvent::ViewMatrixUpdated>(
      [&](const ViewMatrixUpdateEvent& e) {
        callbackInvoked = true;
        receivedCamera = e.camera;
        receivedMatrix = e.view;
      });

  auto viewMatrix = camera.viewMatrix();
  dispatchers.dispatch<CameraEvent::ViewMatrixUpdated>(
      ViewMatrixUpdateEvent{.camera = &camera, .view = viewMatrix});

  EXPECT_TRUE(callbackInvoked);
  EXPECT_EQ(&camera, receivedCamera);
  EXPECT_EQ(viewMatrix, receivedMatrix);
}

TEST_F(CameraEventsTest, CameraDispatchersMultipleListeners) {
  CameraDispatchers dispatchers;
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  int callCount = 0;

  dispatchers.addListener<CameraEvent::ViewMatrixUpdated>(
      [&](const ViewMatrixUpdateEvent&) { callCount++; });

  dispatchers.addListener<CameraEvent::ViewMatrixUpdated>(
      [&](const ViewMatrixUpdateEvent&) { callCount++; });

  dispatchers.dispatch<CameraEvent::ViewMatrixUpdated>(
      ViewMatrixUpdateEvent{.camera = &camera, .view = camera.viewMatrix()});

  EXPECT_EQ(2, callCount);
}

}  // namespace uinta
