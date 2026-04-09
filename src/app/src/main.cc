#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include "uinta/args.h"
#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"
#include "uinta/gl.h"

int main(int argc, const char** argv) {
  uinta::ArgsProcessor args(argc, argv);
  uinta::DesktopPlatform platform;
  uinta::Engine engine({
      .platform = &platform,
      .gl = uinta::OpenGLApiImpl::Instance(),
      .args = &args,
  });
  engine.run();

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
