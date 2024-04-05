#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/app_config_yaml.h"
#include "uinta/args.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/scenes/demo_scene.h"

int main(int argc, const char** argv) {
  uinta::ArgsProcessor args(argc, argv);
  if (!args.status().ok()) {
    LOG(ERROR) << args.status().message();
    exit(args.status().raw_code());
  }

  uinta::AppConfigYamlImpl appConfig(&args);
  if (!appConfig.status().ok()) {
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

    uinta::Engine engine({&platform, &appConfig});
    if (!engine.status().ok()) {
      LOG(ERROR) << absl::StrFormat("Failed to initialize `Engine`: %s",
                                    engine.status().message());
      exit(engine.status().raw_code());
    }

    engine.addScene<uinta::DemoScene>();
    engine.run();
    if (!engine.status().ok()) {
      LOG(ERROR) << absl::StrFormat(
          "`Engine::runScene<T>()` %s failure: %s",
          engine.scenes()->empty()
              ? ""
              : absl::StrFormat("with %s", engine.scenes()->front()->name()),
          engine.status().message());
      exit(engine.status().raw_code());
    }
  }

  appConfig.flush();
  if (!appConfig.status().ok()) {
    LOG(ERROR) << appConfig.status().message();
    exit(appConfig.status().raw_code());
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
