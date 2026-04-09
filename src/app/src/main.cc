#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include "uinta/args.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"

int main(int argc, const char** argv) {
  uinta::ServiceRegistry serviceRegistry;
  serviceRegistry.registerService<const uinta::OpenGLApi>(uinta::OpenGLApiImpl::Instance());

  uinta::ArgsProcessor args(argc, argv);
  if (args.status().ok()) {
    serviceRegistry.registerService<const uinta::ArgsProcessor>(&args);
  } else {
    if (!args.status().message().empty()) {
      LOG(ERROR) << args.status().message();
    }
    exit(args.status().raw_code());
  }

  uinta::DesktopPlatform platform;
  if (!platform.status().ok()) {
    LOG(ERROR) << absl::StrFormat("Failed to initialize `DesktopPlatform`: %s", platform.status().message());
    exit(platform.status().raw_code());
  }

  uinta::Engine engine({
      .serviceRegistry = &serviceRegistry,
      .platform = &platform,
  });
  if (!engine.status().ok()) {
    LOG(ERROR) << absl::StrFormat("Failed to initialize `Engine`: %s", engine.status().message());
    exit(engine.status().raw_code());
  }

  engine.run();
  if (!engine.status().ok()) {
    LOG(ERROR) << absl::StrFormat("`Engine::run()` failure: %s", engine.status().message());
    exit(engine.status().raw_code());
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
