#ifndef UINTA_BUFFER_HPP
#define UINTA_BUFFER_HPP

#include <uinta/gl.h>

namespace uinta {

struct GpuMemoryRegion {
  GLuint id;
  unsigned int count;
  unsigned int max;
  unsigned int offset;

  GpuMemoryRegion() noexcept {
    id = 0;
    count = 0;
    max = 0;
    offset = 0;
  }

  GpuMemoryRegion(const GpuMemoryRegion& other) noexcept { *this = other; }

  GpuMemoryRegion& operator=(const GpuMemoryRegion& other) noexcept {
    id = other.id;
    max = other.max;
    count = other.count;
    offset = other.offset;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_BUFFER_HPP
