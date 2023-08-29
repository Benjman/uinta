#ifndef UINTA_MONITOR_HPP
#define UINTA_MONITOR_HPP

#include <uinta/types.h>

#include <string>

namespace uinta {

template <typename T>
struct Monitor {
  enum class Connection { Disconnected = 0, Connected = 1 };
  T ptr;
  std::string name;
  i32 width;
  i32 height;
  i32 refreshRate;
  Connection connection = Connection::Connected;

  Monitor(T ptr = nullptr, const std::string& name = "", i32 width = 0, i32 height = 0, i32 refreshRate = 0)
      : ptr(ptr), name(name), width(width), height(height), refreshRate(refreshRate) {
  }

  Monitor(const Monitor& other) {
    *this = other;
  }

  Monitor& operator=(const Monitor& rhs) {
    ptr = rhs.ptr;
    name = rhs.name;
    width = rhs.width;
    height = rhs.height;
    refreshRate = rhs.refreshRate;
    connection = rhs.connection;
    return *this;
  }
};

}  // namespace uinta

#endif  // UINTA_MONITOR_HPP
