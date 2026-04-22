#include <absl/log/log.h>
#include <absl/status/status.h>
#include <absl/strings/str_format.h>

#include "uinta/app_config_yaml.h"
#include "uinta/args.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"
#include "uinta/localization/locale.h"

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

  uinta::AppConfigYamlImpl appConfig(&args);
  if (appConfig.status().ok() && !absl::IsNotFound(appConfig.status())) {
    serviceRegistry.registerService<const uinta::AppConfig>(&appConfig);
  } else {
    LOG(ERROR) << appConfig.status().message();
    exit(appConfig.status().raw_code());
  }

  {  // Scoping for app config serializing
    uinta::DesktopPlatform platform(&appConfig);
    if (!platform.status().ok()) {
      LOG(ERROR) << absl::StrFormat("Failed to initialize `DesktopPlatform`: %s", platform.status().message());
      exit(platform.status().raw_code());
    }

    uinta::Engine engine({
        .serviceRegistry = &serviceRegistry,
        .platform = &platform,
        .appConfig = &appConfig,
        .locale = uinta::Locale::EnUs,
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
  }

  appConfig.flush();
  if (!appConfig.status().ok()) {
    LOG(ERROR) << appConfig.status().message();
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
