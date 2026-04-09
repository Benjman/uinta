#include "uinta/engine/engine.h"

#include <absl/log/log.h>

#include <cassert>
#include <string>

namespace uinta {

std::string Engine::Message() const noexcept {
  LOG(INFO) << "Message request recieved.";
  return "Hello from Uinta Engine\n";
}

}  // namespace uinta
