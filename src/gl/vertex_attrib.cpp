#include <uinta/gl/api.h>

#include <uinta/gl/vertex_attrib.hpp>

namespace uinta {

VertexAttrib::VertexAttrib() : VertexAttrib(GL_ZERO, 0, GL_TYPE, GL_FALSE, GL_ZERO, (size_t)GL_ZERO) {
}

VertexAttrib::VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, int32_t offset)
    : VertexAttrib(index, size, type, normalized, stride, (size_t)offset) {
}

VertexAttrib::VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, uint32_t offset)
    : VertexAttrib(index, size, type, normalized, stride, (size_t)offset) {
}

VertexAttrib::VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, size_t offset)
    : VertexAttrib(index, size, type, normalized, stride, reinterpret_cast<const void*>(offset)) {
}

VertexAttrib::VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, const void* pointer)
    : index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer) {
}

VertexAttrib::VertexAttrib(const VertexAttrib& other) {
  *this = other;
}

VertexAttrib& VertexAttrib::operator=(const VertexAttrib& rhs) {
  index = rhs.index;
  size = rhs.size;
  type = rhs.type;
  normalized = rhs.normalized;
  stride = rhs.stride;
  pointer = rhs.pointer;
  return *this;
}

}  // namespace uinta
