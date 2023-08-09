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

  explicit Display(const std::string& title, uint width = 1920, uint height = 1080) noexcept;
};

}  // namespace uinta

#endif  // UINTA_RUNNER_DISPLAY_HPP
