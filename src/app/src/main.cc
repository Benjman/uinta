#include "./demo.h"
#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"

int main(int argc, const char** argv) {
  uinta::ArgsProcessor args(argc, argv);
  uinta::WindowConfig cfg;
  uinta::DesktopPlatform platform(cfg, &args);
  auto gl = uinta::OpenGLApiImpl::Instance();

  if (!platform.status().ok())
    LOG(FATAL) << uinta::StrFormat("Failed to initialize `DesktopPlatform`: %s",
                                   platform.status().message());

  uinta::Engine engine(&platform, gl);
  if (!engine.status().ok())
    LOG(FATAL) << uinta::StrFormat("Failed to initialize `Engine`: %s",
                                   engine.status().message());

  engine.addScene<uinta::DemoScene>(nullptr, &engine);
  engine.run();
  if (!engine.status().ok()) {
    auto msg =
        absl::StrCat("`Engine::run()` failure: ", engine.status().message());

    if (!engine.scenes()->empty()) {
      msg = absl::StrCat(msg, "\n\tWith active scene: \"",
                         engine.scenes()->front()->name(), "\"");
    }

    LOG(FATAL) << msg;
  }

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
