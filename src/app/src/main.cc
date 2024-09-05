#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"

int main(int, const char**) {
  uinta::DesktopPlatform platform;
  if (!platform.status().ok()) {
    LOG(ERROR) << absl::StrFormat("Failed to initialize `DesktopPlatform`: %s",
                                  platform.status().message());
    exit(platform.status().raw_code());
  }

  uinta::Engine engine({&platform});
  if (!engine.status().ok()) {
    LOG(ERROR) << absl::StrFormat("Failed to initialize `Engine`: %s",
                                  engine.status().message());
    exit(engine.status().raw_code());
  }

  engine.run();
  if (!engine.status().ok()) {
    LOG(ERROR) << absl::StrFormat("`Engine::run()` failure: %s",
                                  engine.status().message());
    exit(engine.status().raw_code());
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
