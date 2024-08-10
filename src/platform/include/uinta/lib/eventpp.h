#ifndef SRC_PLATFORM_INCLUDE_UINTA_LIB_EVENTPP_H_
#define SRC_PLATFORM_INCLUDE_UINTA_LIB_EVENTPP_H_

#include <utility>

#include "eventpp/eventdispatcher.h"

namespace uinta {

using eventpp::EventDispatcher;

template <typename Derived, typename S>
class Dispatcher {
 public:
  template <S T, typename... Args>
  void addListener(Args&&... args) noexcept {
    static_cast<Derived*>(this)->dispatchers()->template addListener<T>(
        std::forward<Args>(args)...);
  }

  template <S T, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    static_cast<const Derived*>(this)->dispatchers()->template dispatch<T>(
        std::forward<Args>(args)...);
  }

 protected:
  ~Dispatcher() noexcept = default;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_LIB_EVENTPP_H_
