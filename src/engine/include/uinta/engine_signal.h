#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_SIGNAL_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_SIGNAL_H_

#include <functional>

#include "uinta/types.h"

namespace uinta {

struct ViewportSizeChange {
  u32 width;
  u32 height;

  f32 aspect() const noexcept {
    return static_cast<f32>(width) / static_cast<f32>(height);
  }
};

using ViewportSizeChangeFunc = std::function<void(ViewportSizeChange)>;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_SIGNAL_H_
