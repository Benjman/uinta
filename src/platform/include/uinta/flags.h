#ifndef SRC_PLATFORM_INCLUDE_UINTA_FLAGS_H_
#define SRC_PLATFORM_INCLUDE_UINTA_FLAGS_H_

#define FlagsOperations(DefaultFlags)                                \
  Flags(value_type flags = DefaultFlags) noexcept : flags_(flags) {} \
  ~Flags() noexcept = default;                                       \
  Flags(const Flags& other) noexcept : Flags(other.flags_) {}        \
  Flags(Flags&& other) noexcept : flags_(other.flags_) {}            \
  Flags& operator=(const Flags& other) noexcept {                    \
    if (this == &other) return *this;                                \
    flags_ = other.flags_;                                           \
    return *this;                                                    \
  }                                                                  \
  Flags& operator=(Flags&& other) noexcept {                         \
    flags_ = other.flags_;                                           \
    return *this;                                                    \
  }                                                                  \
  operator value_type() const noexcept { return flags_; }            \
  template <typename T>                                              \
  T as() const noexcept {                                            \
    return static_cast<T>(flags_);                                   \
  }

#define AtomicFlagsOperations(DefaultFlags)                                \
  Flags(value_type flags = DefaultFlags) noexcept { flags_.store(flags); } \
  ~Flags() noexcept = default;                                             \
  Flags(const Flags& other) noexcept : Flags(other.flags_) {}              \
  Flags(Flags&& other) noexcept { flags_.store(other.flags_); }            \
  Flags& operator=(const Flags& other) noexcept {                          \
    if (this == &other) return *this;                                      \
    flags_.store(other.flags_);                                            \
    return *this;                                                          \
  }                                                                        \
  Flags& operator=(Flags&& other) noexcept {                               \
    flags_.store(other.flags_);                                            \
    return *this;                                                          \
  }                                                                        \
  operator value_type() const noexcept { return flags_.load(); }           \
  template <typename T>                                                    \
  T as() const noexcept {                                                  \
    return static_cast<T>(flags_.load());                                  \
  }

#endif  // SRC_PLATFORM_INCLUDE_UINTA_FLAGS_H_
