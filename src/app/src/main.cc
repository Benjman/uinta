#include "absl/log/log.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine.h"

int main() {
  uinta::WindowConfig cfg;
  uinta::DesktopPlatform platform(cfg);

  if (!platform.status().ok())
    LOG(FATAL) << absl::StrFormat("Failed to initialize `DesktopPlatform`: %s",
                                  platform.status().message());

  uinta::Engine engine(&platform);

  if (!engine.status().ok())
    LOG(FATAL) << absl::StrFormat("Engine failure: %s",
                                  engine.status().message());

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
