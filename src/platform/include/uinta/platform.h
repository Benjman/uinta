#ifndef SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_H_
#define SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_H_

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "uinta/args_processor.h"
#include "uinta/lib/absl/status.h"
#include "uinta/monitor.h"
#include "uinta/platform_events.h"
#include "uinta/types.h"
#include "uinta/window.h"

namespace uinta {

class ArgsProcessor;
class Input;

class Platform {
 public:
  explicit Platform(const ArgsProcessor* args) noexcept : args_(args) {}

  virtual ~Platform() noexcept = default;
  Platform(const Platform&) noexcept = delete;
  Platform& operator=(const Platform&) noexcept = delete;
  Platform(Platform&&) noexcept = delete;
  Platform& operator=(Platform&&) noexcept = delete;

  template <PlatformEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  template <PlatformEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    dispatchers_.template dispatch<E>(std::forward<Args>(args)...);
  }

  const Window* window() const noexcept { return window_.get(); }

  Window* window() noexcept { return window_.get(); }

  Monitor* addMonitor(Monitor monitor) noexcept {
    return &monitors_.emplace_back(monitor);
  }

  std::span<const Monitor> monitors() const noexcept { return monitors_; }

  std::optional<Monitor*> findMonitor(void* monitorPtr) noexcept {
    const auto& itr = std::find_if(monitors_.begin(), monitors_.end(),
                                   [monitorPtr](auto& monitor) {
                                     return monitor.userData() == monitorPtr;
                                   });
    if (itr == monitors_.end()) return std::nullopt;
    return &(*itr);
  }

  const std::optional<const Monitor*> primaryMonitor() const noexcept {
    for (auto& monitor : monitors_)
      if (monitor.flags().isPrimary()) return &monitor;
    return std::nullopt;
  }

  virtual void getAndUpdateWindowSize(i32* w, i32* h) const noexcept = 0;

  virtual Status registerInputHandlers(Input*) const noexcept = 0;

  virtual Status pollEvents() const noexcept = 0;

  virtual StatusOr<time_t> runtime() const noexcept = 0;

  virtual Status swapBuffers() const noexcept = 0;

  const Status& status() const noexcept { return status_; }

  const ArgsProcessor* args() const noexcept { return args_; }

 protected:
  PlatformDispatchers dispatchers_;
  std::vector<Monitor> monitors_;
  std::unique_ptr<Window> window_;
  Status status_;

  const ArgsProcessor* args_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_H_
