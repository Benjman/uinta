#ifndef UINTA_RUNNER_WINDOW_HPP
#define UINTA_RUNNER_WINDOW_HPP

#include <uinta/types.h>

#include <string>

namespace uinta {

struct Window {
  std::string title;
  u32 width;
  u32 height;
  f32 aspect_ratio;
  bool fullscreen;

  Window(const std::string& title, u32 width = 0, u32 height = 0, bool fullscreen = false) noexcept
      : title(std::string(title)),
        width(width),
        height(height),
        aspect_ratio(static_cast<f32>(width) / static_cast<f32>(height)),
        fullscreen(fullscreen) {
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_WINDOW_HPP
