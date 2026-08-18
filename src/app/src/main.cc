#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include "uinta/desktop_platform.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"

int main() {
  uinta::DesktopPlatform platform;
  uinta::Engine engine({
      .platform = &platform,
      .gl = uinta::OpenGLApiImpl::Instance(),
  });
  engine.run();

  LOG(INFO) << "Exiting";
  return EXIT_SUCCESS;
}
