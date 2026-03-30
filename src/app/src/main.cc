#include <absl/log/log.h>
#include <absl/status/status.h>
#include <absl/strings/str_format.h>

#include "uinta/app_config_yaml.h"
#include "uinta/args.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"
#include "uinta/gl.h"
#include "uinta/scenes/demo_scene.h"
#include "uinta/viewport/viewport_manager.h"

int main(int argc, const char** argv) {
  uinta::ArgsProcessor args(argc, argv);
  uinta::AppConfigYamlImpl appConfig(&args);

  {  // Scoping for app config serializing
    uinta::DesktopPlatform platform(&appConfig);
    uinta::ViewportManager viewport(&appConfig);
    uinta::Engine engine({
        .platform = &platform,
        .gl = uinta::OpenGLApiImpl::Instance(),
        .appConfig = &appConfig,
        .args = &args,
        .viewport = &viewport,
    });
    engine.addScene<uinta::DemoScene>();
    engine.run();
  }

  if (auto status = appConfig.flush(); !status.ok()) {
    LOG(FATAL) << status.message();
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
