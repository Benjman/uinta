#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_RUNTIME_GETTER_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_RUNTIME_GETTER_H_

#include "uinta/types.h"

namespace uinta {

class RuntimeGetter {
 public:
  virtual time_t runtime() const noexcept = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_RUNTIME_GETTER_H_
