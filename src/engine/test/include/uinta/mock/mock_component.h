#ifndef SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_COMPONENT_H_
#define SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_COMPONENT_H_

#include <gtest/gtest.h>

#include "uinta/component.h"

namespace uinta {

template <typename T>
class MockComponent : public T {
 public:
  void update(time_t) noexcept override { updateCalled = true; }

  bool updateCalled = false;
};

class MockIntervalComponent : public IntervalComponent {
 public:
  explicit MockIntervalComponent(time_t interval = 1,
                                 bool isOneshot = false) noexcept
      : IntervalComponent(interval, isOneshot) {}

  void update(time_t) noexcept override { updateCalled = true; }

  bool updateCalled = false;
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_COMPONENT_H_
