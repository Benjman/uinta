#ifndef SRC_PLATFORM_INCLUDE_UINTA_MONITOR_H_
#define SRC_PLATFORM_INCLUDE_UINTA_MONITOR_H_

#include <string>

#include "uinta/flags.h"
#include "uinta/types.h"

namespace uinta {

class Monitor {
 public:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(0);

    bool isPrimary() const noexcept { return flags_ & PrimaryMask; }
    void isPrimary(bool v) noexcept {
      flags_ &= ~PrimaryMask;
      if (v) flags_ |= PrimaryMask;
    }

    bool isConnected() const noexcept { return flags_ & ConnectionMask; }
    void isConnected(bool v) noexcept {
      flags_ &= ~ConnectionMask;
      if (v) flags_ |= ConnectionMask;
    }

   private:
    static constexpr value_type PrimaryMask = 1 << 0;
    static constexpr value_type ConnectionMask = 1 << 1;

    value_type flags_;
  };

  Monitor(std::string_view name = "", i32 width = 0, i32 height = 0, i32 hz = 0,
          void* userData = nullptr, bool isPrimary = false,
          bool isConnected = true)
      : name_(name),
        width_(width),
        height_(height),
        hz_(hz),
        userData_(userData) {
    flags_.isPrimary(isPrimary);
    flags_.isConnected(isConnected);
  }

  Flags flags() const noexcept { return flags_; }

  i32 height() const noexcept { return height_; }

  i32 hz() const noexcept { return hz_; }

  std::string name() const noexcept { return name_; }

  void* userData() const noexcept { return userData_; }

  i32 width() const noexcept { return width_; }

 private:
  std::string name_;
  i32 width_;
  i32 height_;
  i32 hz_;
  void* userData_;
  Flags flags_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_MONITOR_H_
