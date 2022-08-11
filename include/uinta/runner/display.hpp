#ifndef UINTA_RUNNER_DISPLAY_HPP
#define UINTA_RUNNER_DISPLAY_HPP

#include <string>

namespace uinta {

struct Display {
  std::string title;
  unsigned int width;
  unsigned int height;
  float aspectRatio;

  Display() : Display("", 0, 0) {}

  Display(const std::string& title, const unsigned int width, const unsigned int height) noexcept;
};

}  // namespace uinta

#endif  // UINTA_RUNNER_DISPLAY_HPP
