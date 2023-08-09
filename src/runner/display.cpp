#include <uinta/runner/display.hpp>

uinta::Display::Display(const std::string& title, uint width, uint height) noexcept
    : title(std::string(title)), width(width), height(height) {
  aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}
