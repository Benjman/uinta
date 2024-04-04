#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_

#include "uinta/types.h"

namespace uinta {

class Monitor;

struct FrameManager {
  time_t next = 0;
  f32 frequency = 0.0;

  explicit FrameManager(const Monitor*) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_FRAME_MANAGER_H_
