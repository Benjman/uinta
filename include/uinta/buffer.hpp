#ifndef UINTA_BUFFER_HPP
#define UINTA_BUFFER_HPP

#include <uinta/gl.h>

#include <cstdint>

namespace uinta {

struct GpuMemoryArena {
  uint32_t vboId;
  uint32_t count;
  uint32_t max;
  uint32_t offset;

  GpuMemoryArena() noexcept {
    vboId = 0;
    count = 0;
    max = 0;
    offset = 0;
  }

  GpuMemoryArena(const GpuMemoryArena& other) noexcept { *this = other; }

  GpuMemoryArena& operator=(const GpuMemoryArena& other) noexcept {
    vboId = other.vboId;
    max = other.max;
    count = other.count;
    offset = other.offset;
    return *this;
  }
};

struct GpuMemoryContext {
  GLuint id;
  uint32_t count;
  uint32_t offset;

  GpuMemoryContext(GLuint id = 0, uint32_t count = 0, uint32_t offset = 0) : id(id), count(count), offset(offset) {}

  GpuMemoryContext(const GpuMemoryContext& other) { *this = other; }

  GpuMemoryContext& operator=(const GpuMemoryContext& rhs) {
    id = rhs.id;
    count = rhs.count;
    offset = rhs.offset;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_BUFFER_HPP
