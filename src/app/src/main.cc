#include <absl/log/log.h>
#include <absl/status/status.h>
#include <absl/strings/str_format.h>

#include "uinta/app_config_yaml.h"
#include "uinta/args.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"

int main(int argc, const char** argv) {
  uinta::ArgsProcessor args(argc, argv);
  if (!args.status().ok()) {
    if (!args.status().message().empty()) {
      LOG(ERROR) << args.status().message();
    }
    exit(args.status().raw_code());
  }

  uinta::AppConfigYamlImpl appConfig(&args);
  if (!appConfig.status().ok() && !absl::IsNotFound(appConfig.status())) {
    LOG(ERROR) << appConfig.status().message();
    exit(appConfig.status().raw_code());
  }

  {  // Scoping for app config serializing
    uinta::DesktopPlatform platform(&appConfig);
    if (!platform.status().ok()) {
      LOG(ERROR) << absl::StrFormat(
          "Failed to initialize `DesktopPlatform`: %s",
          platform.status().message());
      exit(platform.status().raw_code());
    }

    uinta::Engine engine({
        .platform = &platform,
        .appConfig = &appConfig,
        .args = &args,
        .gl = uinta::OpenGLApiImpl::Instance(),
    });
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
  }

  appConfig.flush();
  if (!appConfig.status().ok()) {
    LOG(ERROR) << appConfig.status().message();
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
