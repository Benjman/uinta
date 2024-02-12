#ifndef SRC_ENGINE_TEST_SRC_MOCK_MOCK_RUNTIME_GETTER_H_
#define SRC_ENGINE_TEST_SRC_MOCK_MOCK_RUNTIME_GETTER_H_

#include <functional>

#include "uinta/runtime_getter.h"

namespace uinta {

class MockRuntimeGetter : public RuntimeGetter {
 public:
  static constexpr time_t RuntimeStep = 1.0 / 144.0;
  mutable time_t runtime_ = 0;
  std::function<time_t()> onRuntime = [this] -> time_t { return runtime_; };
  time_t runtime() const noexcept override {
    runtime_ += RuntimeStep;
    return onRuntime();
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_MOCK_MOCK_RUNTIME_GETTER_H_
