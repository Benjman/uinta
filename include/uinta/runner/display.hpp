#ifndef UINTA_RUNNER_DISPLAY_HPP
#define UINTA_RUNNER_DISPLAY_HPP

#include <string>
#include <uinta/fwd.hpp>

namespace uinta {

struct Display {
  std::string title;
  u32 width;
  u32 height;
  f32 aspectRatio;

  Display() : Display("") {
  }

  Display(const Display& other) {
    *this = other;
  }

  explicit Display(const std::string& title, u32 width = 1920, u32 height = 1080) noexcept;

  Display& operator=(const Display& other) {
    this->title = other.title;
    this->width = other.width;
    this->height = other.height;
    this->aspectRatio = other.aspectRatio;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_DISPLAY_HPP
