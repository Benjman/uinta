#ifndef UINTA_HEIGHT_MAP_HPP
#define UINTA_HEIGHT_MAP_HPP

#include <uinta/types.h>

#include <glm/vec2.hpp>

namespace uinta {

template <typename T = f32>
struct Buffer2d {
  T* const buffer;
  const glm::ivec2 size;

  Buffer2d(size_t width, size_t height) : Buffer2d(new T[width * height], {width, height}) {
  }

  Buffer2d(const glm::ivec2& size) : Buffer2d(new T[size.x * size.y], size) {
  }

  Buffer2d(T* const buffer, const glm::ivec2 size) : buffer(buffer), size(size) {
  }

  ~Buffer2d() {
    delete[] buffer;
  }

  T& operator[](const size_t i) {
    return buffer[i];
  }

  T* operator&() const {
    return buffer;
  }

  T& at(const glm::ivec2& coord) {
    return at(coord.x, coord.y);
  }

  T& at(size_t x, size_t y) {
    return buffer[y * size.x + x];
  }
};

}  // namespace uinta

#endif  // UINTA_HEIGHT_MAP_HPP
