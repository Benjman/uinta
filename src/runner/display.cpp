#include "uinta/runner/display.hpp"

uinta::Display::Display(const std::string& title, unsigned int width, unsigned int height) noexcept
    : title(std::string(title)), width(width), height(height) {
  aspectRatio = (float)width / (float)height;
}
