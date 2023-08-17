#ifndef UINTA_RUNNER_DISPLAY_HPP
#define UINTA_RUNNER_DISPLAY_HPP

#include <string>
#include <uinta/fwd.hpp>

namespace uinta {

struct Display {
  std::string title;
  uint width;
  uint height;
  float aspectRatio;

  Display() : Display("") {
  }

  Display(const Display& other) {
    *this = other;
  }

  explicit Display(const std::string& title, uint width = 1920, uint height = 1080) noexcept;

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
