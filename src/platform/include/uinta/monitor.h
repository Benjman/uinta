#ifndef SRC_PLATFORM_INCLUDE_UINTA_MONITOR_H_
#define SRC_PLATFORM_INCLUDE_UINTA_MONITOR_H_

#include <string>

#include "uinta/types.h"

namespace uinta {

class Monitor {
 public:
  enum class Connection : u8 {
    Disconnected = 0,
    Connected = 1,
  };

  Monitor(std::string_view name = "", i32 width = 0, i32 height = 0, i32 hz = 0,
          void* userData = nullptr, bool isPrimary = false)
      : name_(name),
        width_(width),
        height_(height),
        hz_(hz),
        flags_(isPrimary ? IsPrimaryMask : 0),
        userData_(userData),
        connection_(Connection::Connected) {}

  Connection connection() const noexcept { return connection_; }

  i32 height() const noexcept { return height_; }

  i32 hz() const noexcept { return hz_; }

  std::string_view name() const noexcept { return name_; }

  void* userData() const noexcept { return userData_; }

  i32 width() const noexcept { return width_; }

  bool isPrimary() const noexcept { return flags_ & IsPrimaryMask; }

 private:
  bitflag8 IsPrimaryMask = 1 << 0;

  std::string name_;
  i32 width_;
  i32 height_;
  i32 hz_;
  bitflag8 flags_;
  void* userData_;
  Connection connection_;
};

std::string to_string(Monitor::Connection c) noexcept;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_MONITOR_H_
