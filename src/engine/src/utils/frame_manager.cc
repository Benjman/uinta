#include "uinta/utils/frame_manager.h"

#include "uinta/monitor.h"
#include "uinta/platform.h"

namespace uinta {

FrameManager::FrameManager(const Platform* platform)
    : nextFrameAdvance(1.0f /
                       static_cast<f32>(platform->primaryMonitor()->hz())) {}

void FrameManager::onMonitorChange(const Monitor* monitor) noexcept {
  // TODO: We aren't watching for monitor changes. Once that happens, call this
  // function when a monitor change event occurs.
  nextFrameAdvance = 1.0f / static_cast<f32>(monitor->hz());
}

}  // namespace uinta
