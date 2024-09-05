#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_VIEWPORT_SIZE_CHANGE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_VIEWPORT_SIZE_CHANGE_H_

#include "uinta/types.h"

namespace uinta {

struct ViewportSizeChange {
  u32 width;
  u32 height;

  f32 aspect() const noexcept {
    return static_cast<f32>(width) / static_cast<f32>(height);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_VIEWPORT_SIZE_CHANGE_H_
