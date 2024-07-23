#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_GENERATOR_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_GENERATOR_H_

#include <utility>

namespace uinta {

template <typename T, typename... Args>
class Generator {
 public:
  virtual T generate(Args&&... args) const noexcept {
    return T(std::forward<Args>(args)...);
  }

  virtual T generate(Args&&... args) noexcept {
    return T(std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_GENERATOR_H_
