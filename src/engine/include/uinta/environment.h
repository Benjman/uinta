#ifndef SRC_ENGINE_INCLUDE_UINTA_ENVIRONMENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENVIRONMENT_H_

#include "uinta/primitive.h"

namespace uinta {

class Environment {
 public:
  static Primitive Tree(size_t* idxOffset) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENVIRONMENT_H_
