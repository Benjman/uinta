#include <uinta/runner/display.hpp>

uinta::Display::Display(const std::string& title) noexcept : title(std::string(title)) {
  aspectRatio = (float)width / (float)height;
}
