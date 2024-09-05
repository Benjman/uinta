#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_RUNTIME_GETTER_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_RUNTIME_GETTER_H_

#include <functional>

#include "uinta/runtime_getter.h"

namespace uinta {

struct MockRuntimeGetter : RuntimeGetter {
  static constexpr time_t RuntimeStep = 1.0 / 144.0;
  mutable time_t runtime_ = 0;
  std::function<time_t()> onRuntime = [this] { return runtime_; };
  time_t runtime() const noexcept override {
    runtime_ += RuntimeStep;
    return onRuntime();
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_RUNTIME_GETTER_H_
