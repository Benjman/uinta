#include "uinta/engine/engine.h"

#include "absl/log/log.h"

namespace uinta {

const StatusOr<const std::string> Engine::Message() const noexcept {
  LOG(INFO) << "Message request recieved.";
  return "Hello from Uinta Engine\n";
}

}  // namespace uinta
