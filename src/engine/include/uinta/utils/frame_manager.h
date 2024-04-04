#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_

#include "uinta/types.h"

namespace uinta {

class Monitor;
class Platform;

struct FrameManager {
  explicit FrameManager(const Platform*);
  f32 nextFrame = 0;
  f32 nextFrameAdvance;
  void onMonitorChange(const Monitor*) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_
