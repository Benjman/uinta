#include "uinta/desktop_window.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

DesktopWindow::DesktopWindow(DesktopPlatform* platform,
                             WindowConfig& cfg) noexcept
    : Window(cfg, platform), platform_(platform) {
  assert(platform_ && "`Platform*' cannot be null.");

  cfg.apiUserPtr(this);

  if (cfg.isFullscreen() && !cfg.monitor())
    cfg.isFullscreen(true, platform_->primaryMonitor());

  const auto apiWindow = platform_->createWindow(this);
  if (!apiWindow.ok()) {
    status_ = apiWindow.status();
    return;
  }
  userData(apiWindow.value());

  if (status_ = platform_->setWindowPosition(userData(), cfg.x(), cfg.y());
      !status_.ok()) {
    return;
  }
}

DesktopWindow::~DesktopWindow() noexcept {
  auto status = platform_->destroy(this);
  if (!status.ok())
    LOG(ERROR) << StrFormat("Failed to destruct window: %s", status.message());
}

}  // namespace uinta
