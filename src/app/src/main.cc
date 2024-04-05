#include "./demo.h"
#include "absl/log/log.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine.h"

int main() {
  uinta::DesktopPlatform platform;
  if (!platform.status().ok()) {
    LOG(FATAL) << absl::StrFormat("Failed to initialize `DesktopPlatform`: %s",
                                  platform.status().message());
    return platform.status().raw_code();
  }

  uinta::DemoScene scene;
  uinta::Engine engine(&platform, &scene);
  if (!engine.status().ok()) {
    LOG(FATAL) << absl::StrFormat("Engine failure: %s",
                                  engine.status().message());
    return engine.status().raw_code();
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
