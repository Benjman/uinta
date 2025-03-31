#include "uinta/desktop_window.h"

#include <cassert>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"

namespace uinta {

DesktopWindow::DesktopWindow(DesktopPlatform* platform,
                             const AppConfig* appConfig) noexcept
    : Window(platform, appConfig), platform_(platform) {
  if (!status_.ok()) {
    return;
  }

  assert(platform_ && "`Platform*' cannot be null.");

  const auto apiWindow = platform_->createWindow(this);
  if (!apiWindow.ok()) {
    status_ = apiWindow.status();
    return;
  }
  userData(apiWindow.value());

  if (status_ = platform_->setWindowPosition(userData(), x(), y());
      !status_.ok()) {
    return;
  }
}

DesktopWindow::~DesktopWindow() noexcept {
  auto status = platform_->destroy(this);
  if (!status.ok()) {
    LOG(ERROR) << absl::StrFormat("Failed to destruct window: %s",
                                  status.message());
  }
}

}  // namespace uinta
