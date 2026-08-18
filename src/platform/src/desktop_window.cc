#include "uinta/desktop_window.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <cassert>

namespace uinta {

DesktopWindow::DesktopWindow(DesktopPlatform* platform) noexcept : Window(platform), platform_(platform) {
  assert(platform_ && "`Platform*' cannot be null.");

  if (const auto status = platform_->createWindow(this); status.ok()) {
    userData(status.value());
  } else {
    LOG(FATAL) << status.status().message();
  }

  if (auto status = platform_->setWindowPosition(userData(), x(), y()); !status.ok()) {
    LOG(FATAL) << status.message();
  }

  LOG(INFO) << "Initialized desktop window.";
}

DesktopWindow::~DesktopWindow() noexcept {
  auto status = platform_->destroy(this);
  if (!status.ok()) {
    LOG(ERROR) << absl::StrFormat("Failed to destruct window: %s", status.message());
  }
}

}  // namespace uinta
