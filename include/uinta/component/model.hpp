#ifndef UINTA_COMPONENT_MODEL_HPP
#define UINTA_COMPONENT_MODEL_HPP

#include <uinta/types.h>

namespace uinta {

using model_t = u16;

struct Model {
  model_t id;  // TODO move to flags
  u32 vertexCount;
  u32 indexCount;
  u32 indexOffset;

  Model() : id(0), vertexCount(0), indexCount(0), indexOffset(0) {
  }

  Model(const Model& other) {
    *this = other;
  }

  Model& operator=(const Model& rhs) {
    id = rhs.id;
    vertexCount = rhs.vertexCount;
    indexCount = rhs.indexCount;
    indexOffset = rhs.indexOffset;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_COMPONENT_MODEL_HPP
