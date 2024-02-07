#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_

#include <string>

namespace uinta {

class Engine {
 public:
  Engine() noexcept = default;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  Engine(Engine&&) noexcept;
  Engine& operator=(Engine&&) noexcept;

  const std::string Message() const noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_
