#include "uinta/desktop_platform.h"

#include "absl/log/log.h"
#include "uinta/desktop_window.h"

namespace uinta {

DesktopPlatform::DesktopPlatform(WindowConfig config,
                                 DesktopPlatformApi* api) noexcept
    : api_(api) {
  if (status_ = api_->initOpenGL(); !status_.ok()) {
    return;
  }

  if (auto status = api_->findMonitors(); status.ok()) {
    monitors_ = status.value();
  } else {
    status_ = status.status();
    return;
  }

  window_ = std::make_unique<DesktopWindow>(this, config);

  if (status_ = window_->status(); status_.ok())
    LOG(INFO) << "Initialized Desktop Platform.";

  if (!config.isFullscreen()) {
    i32 w, h;
    getAndUpdateWindowSize(&w, &h);
  }
}

DesktopPlatform::~DesktopPlatform() noexcept {
  if (status_ = destroy(window_.get()); !status_.ok())
    LOG(ERROR) << status_.message();
}

}  // namespace uinta
