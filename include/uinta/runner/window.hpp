#ifndef UINTA_RUNNER_WINDOW_HPP
#define UINTA_RUNNER_WINDOW_HPP

#include <uinta/flags.h>
#include <uinta/types.h>

#include <string>

namespace uinta {

struct Window {
  static constexpr flag_t FULLSCREEN = 1 << 1;

  std::string title;
  u32 width;
  u32 height;
  f32 aspectRatio;
  flags_t flags = 0;

  Window(const Window& other) {
    *this = other;
  }

  explicit Window(const std::string& title, u32 width = 1920, u32 height = 1080) noexcept
      : title(std::string(title)), width(width), height(height) {
    aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
  }

  Window& operator=(const Window& other) {
    this->title = other.title;
    this->width = other.width;
    this->height = other.height;
    this->aspectRatio = other.aspectRatio;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_WINDOW_HPP
