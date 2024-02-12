#ifndef SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_SIGNAL_H_
#define SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_SIGNAL_H_

#include <functional>

#include "uinta/types.h"

namespace uinta {

using CloseRequestFunc = std::function<void(void *)>;

using DebugMessageFunc =
    std::function<void(u32, u32, u32, u32, size_t, const char *, const void *)>;

using ErrorFunc = std::function<void(i32, const char *)>;

using FramebufferSizeFunc = std::function<void(u32, u32)>;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_SIGNAL_H_
