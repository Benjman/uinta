#include "uinta/platform.h"

#include <gtest/gtest.h>

#include "uinta/window.h"

namespace uinta {

TEST(WindowConfig, IsFullscreen) {
  WindowConfig config;
  ASSERT_TRUE(config.isFullscreen());
}

}  // namespace uinta
