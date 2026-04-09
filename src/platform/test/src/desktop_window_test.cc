#include <gtest/gtest.h>

#include "uinta/desktop_platform.h"
#include "uinta/mock/mock_desktop_platform_api.h"

namespace uinta {

class DesktopWindowTestF : public ::testing::Test {
 protected:
  MockDesktopPlatformApi api;
};

TEST_F(DesktopWindowTestF, ConstructorSuccess) {
  DesktopPlatform platform(&api);
  ASSERT_TRUE(platform.status().ok());

  // Window is created as part of DesktopPlatform construction
  const Window* window = platform.window();
  EXPECT_NE(window, nullptr);
  EXPECT_TRUE(window->status().ok());
}

TEST_F(DesktopWindowTestF, ConstructorStoresUserData) {
  void* expectedHandle = reinterpret_cast<void*>(0x12345678);
  api.onCreateWindow = [expectedHandle](Window*) -> StatusOr<void*> {
    return expectedHandle;
  };

  DesktopPlatform platform(&api);
  ASSERT_TRUE(platform.status().ok());

  EXPECT_EQ(platform.window()->userData(), expectedHandle);
}

TEST_F(DesktopWindowTestF, ConstructorCreateWindowFails) {
  api.onCreateWindow = [](Window*) -> StatusOr<void*> {
    return InternalError("Window creation failed");
  };

  DesktopPlatform platform(&api);

  EXPECT_FALSE(platform.status().ok());
  EXPECT_TRUE(IsInternal(platform.status()));
}

TEST_F(DesktopWindowTestF, ConstructorSetWindowPositionFails) {
  api.onSetWindowPosition = [](void*, f32, f32) -> Status {
    return InternalError("Failed to set window position");
  };

  DesktopPlatform platform(&api);

  EXPECT_FALSE(platform.status().ok());
  EXPECT_TRUE(IsInternal(platform.status()));
}

TEST_F(DesktopWindowTestF, ConstructorCallsSetWindowPosition) {
  bool setPositionCalled = false;
  void* capturedWindow = nullptr;
  f32 capturedX = -1;
  f32 capturedY = -1;

  api.onSetWindowPosition = [&](void* w, f32 x, f32 y) -> Status {
    setPositionCalled = true;
    capturedWindow = w;
    capturedX = x;
    capturedY = y;
    return OkStatus();
  };

  void* windowHandle = reinterpret_cast<void*>(0xABCD);
  api.onCreateWindow = [windowHandle](Window*) -> StatusOr<void*> {
    return windowHandle;
  };

  DesktopPlatform platform(&api);
  ASSERT_TRUE(platform.status().ok());

  EXPECT_TRUE(setPositionCalled);
  EXPECT_EQ(capturedWindow, windowHandle);
  // Default position is 0, 0
  EXPECT_EQ(capturedX, 0);
  EXPECT_EQ(capturedY, 0);
}

TEST_F(DesktopWindowTestF, DestructorCallsDestroy) {
  bool destroyCalled = false;
  const Window* capturedWindow = nullptr;

  api.onDestroy = [&](const Window* w) -> Status {
    destroyCalled = true;
    capturedWindow = w;
    return OkStatus();
  };

  {
    DesktopPlatform platform(&api);
    ASSERT_TRUE(platform.status().ok());
  }  // Destructor called here

  EXPECT_TRUE(destroyCalled);
  // capturedWindow pointer is dangling now, but we can verify it was called
}

TEST_F(DesktopWindowTestF, DestructorHandlesDestroyFailure) {
  // This test verifies that destroy failure is logged but doesn't throw
  api.onDestroy = [](const Window*) -> Status {
    return InternalError("Destroy failed");
  };

  // Should not throw or crash
  {
    DesktopPlatform platform(&api);
    ASSERT_TRUE(platform.status().ok());
  }  // Destructor logs error but doesn't throw
}

}  // namespace uinta
