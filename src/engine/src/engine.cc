#include "uinta/engine/engine.h"

#include <cassert>
#include <string>

namespace uinta {

std::string Engine::Message() const noexcept {
  return "Hello from Uinta Engine\n";
}

}  // namespace uinta
