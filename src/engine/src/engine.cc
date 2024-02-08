#include "uinta/engine.h"

namespace uinta {

const StatusOr<const std::string> Engine::Message() const noexcept {
  return "Hello from Uinta Engine\n";
}

Engine::Engine(Engine&&) noexcept {}

Engine& Engine::operator=(Engine&&) noexcept { return *this; }

}  // namespace uinta
