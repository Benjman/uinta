#include "uinta/desktop_platform.h"

#include "absl/log/log.h"
#include "uinta/desktop_window.h"

namespace uinta {

DesktopPlatform::DesktopPlatform(WindowConfig config,
                                 DesktopPlatformApi* api) noexcept
    : api_(api) {
  status_ = api_->initOpenGL();
  if (!status_.ok()) return;

  auto findMonitorsStatus = api_->findMonitors();
  status_ = findMonitorsStatus.status();
  if (!status_.ok()) return;

  monitors_ = findMonitorsStatus.value();

  window_ = std::make_unique<DesktopWindow>(this, config);
  if (status_ = window_->status(); status_.ok())
    LOG(INFO) << "Initialized Desktop Platform.";
}

DesktopPlatform::~DesktopPlatform() noexcept {
  status_ = destroy(window_.get());
  if (!status_.ok()) LOG(FATAL) << status_.message();
}

}  // namespace uinta
