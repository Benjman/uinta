#include "uinta/monitor.h"

#include "absl/log/log.h"

namespace uinta {

inline std::string to_string(Monitor::Connection c) noexcept {
  switch (c) {
    case (Monitor::Connection::Disconnected):
      return "Disconnected";
    case (Monitor::Connection::Connected):
      return "Connected";
    default:
      LOG(FATAL) << "Invalid value in switch statement.";
  }
}

}  // namespace uinta
