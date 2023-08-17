#include <uinta/runner/display.hpp>

uinta::Display::Display(const std::string& title, u32 width, u32 height) noexcept
    : title(std::string(title)), width(width), height(height) {
  aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
}
