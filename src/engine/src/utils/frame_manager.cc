#include "uinta/utils/frame_manager.h"

#include "absl/log/log.h"
#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

FrameManager::FrameManager(const Platform* platform) noexcept {
  onMonitorChange(platform->primaryMonitor().value_or(nullptr));
}

void FrameManager::onMonitorChange(const Monitor* monitor) noexcept {
  if (!monitor) {
    LOG(WARNING) << "Unable to set nextFrameAdvance because Monitor* was null";
    return;
  }
  nextFrameAdvance = 1.0f / static_cast<f32>(monitor->hz());
}

}  // namespace uinta
