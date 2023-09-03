#ifndef UINTA_RUNNER_WINDOW_HPP
#define UINTA_RUNNER_WINDOW_HPP

#include <uinta/types.h>

#include <string>

namespace uinta {

struct Window {
  std::string title = "";
  u32 width = 0;
  u32 height = 0;
  f32 aspect_ratio = 1;
  bool fullscreen = false;

  Window(const std::string& title, u32 width = 0, u32 height = 0, bool fullscreen = false) noexcept
      : title(std::string(title)), width(width), height(height), fullscreen(fullscreen) {
    aspect_ratio = static_cast<f32>(width) / static_cast<f32>(height);
  }

  Window(const Window& other) noexcept {
    *this = other;
  }

  Window& operator=(const Window& other) noexcept {
    title = other.title;
    width = other.width;
    height = other.height;
    aspect_ratio = static_cast<f32>(width) / static_cast<f32>(height);
    fullscreen = other.fullscreen;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_WINDOW_HPP
