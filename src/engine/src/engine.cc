#include "uinta/engine/engine.h"

namespace uinta {

const StatusOr<const std::string> Engine::Message() const noexcept {
  return "Hello from Uinta Engine\n";
}

}  // namespace uinta
