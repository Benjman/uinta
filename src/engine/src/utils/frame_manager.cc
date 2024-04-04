#include "uinta/utils/frame_manager.h"

#include "absl/log/log.h"
#include "uinta/monitor.h"

namespace uinta {

FrameManager::FrameManager(const Monitor* monitor) noexcept {
  if (monitor) {
    frequency = 1.0 / static_cast<time_t>(monitor->hz());
  } else {
    frequency = 1.0 / 60;
    LOG(WARNING) << "Unable to set nextFrameAdvance because Monitor* was null";
  }
}

}  // namespace uinta
