#ifndef UINTA_RUNNER_DISPLAY_HPP
#define UINTA_RUNNER_DISPLAY_HPP

#include <string>
#include <uinta/fwd.hpp>

namespace uinta {

struct Display {
  std::string title;
  uint32_t width;
  uint32_t height;
  float aspectRatio;

  Display() : Display("", 0, 0) {
  }

  Display(const std::string& title, const uint32_t width, const uint32_t height) noexcept;
};

}  // namespace uinta

#endif  // UINTA_RUNNER_DISPLAY_HPP
