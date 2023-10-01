#ifndef UINTA_MONITOR_HPP
#define UINTA_MONITOR_HPP

#include <uinta/types.h>

#include <string>

namespace uinta {

struct Monitor {
  enum class Connection {
    Disconnected = 0,
    Connected = 1,
  };

  void* ptr;
  std::string name;
  i32 width;
  i32 height;
  i32 refreshRate;
  Connection connection;

  Monitor(void* ptr = nullptr, const std::string& name = "", i32 width = 0, i32 height = 0, i32 refreshRate = 0)
      : ptr(ptr), name(name), width(width), height(height), refreshRate(refreshRate), connection(Connection::Connected) {
  }
};

inline std::string to_string(Monitor::Connection connection) {
  switch (connection) {
    case (Monitor::Connection::Disconnected):
      return "Disconnected";
    case (Monitor::Connection::Connected):
      return "Connected";
    default:
      return "Unknown";
  };
}

}  // namespace uinta

#endif  // UINTA_MONITOR_HPP
