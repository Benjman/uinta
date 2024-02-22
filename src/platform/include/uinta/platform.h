#ifndef SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_H_
#define SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_H_

#include <memory>
#include <vector>

#include "uinta/lib/absl/status.h"
#include "uinta/monitor.h"
#include "uinta/platform_signal.h"
#include "uinta/types.h"
#include "uinta/window.h"

namespace uinta {

class Input;

class Platform {
 public:
  Platform() noexcept = default;
  virtual ~Platform() noexcept = default;
  Platform(const Platform&) noexcept = delete;
  Platform& operator=(const Platform&) noexcept = delete;
  Platform(Platform&&) noexcept = delete;
  Platform& operator=(Platform&&) noexcept = delete;

  const Window* window() const noexcept { return window_.get(); }

  std::span<const Monitor> monitors() const noexcept { return monitors_; }

  const Monitor* primaryMonitor() const noexcept {
    for (auto& monitor : monitors_)
      if (monitor.isPrimary()) return &monitor;
    return nullptr;
  }

  virtual Status clearFrontBuffer(f32, f32, f32, f32,
                                  bitflag) const noexcept = 0;

  virtual void getWindowSize(const Window*, i32* w, i32* h) const noexcept = 0;

  virtual void onCloseRequest(const CloseRequestFunc&) noexcept = 0;

  virtual void onDebugMessage(const DebugMessageFunc&) noexcept = 0;

  virtual void onError(const ErrorFunc&) noexcept = 0;

  virtual void onFramebufferSize(const FramebufferSizeFunc&) noexcept = 0;

  virtual Status pollEvents() const noexcept = 0;

  virtual StatusOr<f64> runtime() const noexcept = 0;

  virtual Status swapBuffers() const noexcept = 0;

  const Status& status() const noexcept { return status_; }

  virtual Status registerInputHandlers(Input*) const noexcept = 0;

 protected:
  std::vector<Monitor> monitors_;
  std::unique_ptr<Window> window_;
  Status status_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_H_
