#ifndef UINTA_VERTEX_ATTRIB_HPP
#define UINTA_VERTEX_ATTRIB_HPP

#include <uinta/types.h>

namespace uinta {

struct VertexAttrib {
  u32 index;
  i32 size;
  u32 type;
  bool normalized;
  i32 stride;
  const void* pointer;

  VertexAttrib();
  VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, const void* pointer);
  VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, int32_t offset);
  VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, size_t offset);
  VertexAttrib(u32 index, i32 size, u32 type, bool normalized, i32 stride, uint32_t offset);
  VertexAttrib(const VertexAttrib& other);

  VertexAttrib& operator=(const VertexAttrib& rhs);
};

}  // namespace uinta

#endif  // UINTA_VERTEX_ATTRIB_HPP
