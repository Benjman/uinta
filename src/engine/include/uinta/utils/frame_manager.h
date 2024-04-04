#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_

#include "uinta/types.h"

namespace uinta {

class Monitor;
class Platform;

struct FrameManager {
  time_t nextFrame = 0;
  time_t nextFrameAdvance = 1.0 / 60.0;

  explicit FrameManager(const Platform*) noexcept;

  void onMonitorChange(const Monitor*) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_
