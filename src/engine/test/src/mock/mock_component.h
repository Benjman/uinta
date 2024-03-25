#ifndef SRC_ENGINE_TEST_SRC_MOCK_MOCK_COMPONENT_H_
#define SRC_ENGINE_TEST_SRC_MOCK_MOCK_COMPONENT_H_

#include <gtest/gtest.h>

#include <set>

#include "uinta/component.h"
#include "uinta/engine/engine_state.h"

namespace uinta {

template <typename T>
class MockComponent : public T {
 public:
  void update(const EngineState& state) noexcept override {
    updateCalled = true;
    ASSERT_EQ(calledFromTick.end(), calledFromTick.find(state.tick()));
    ASSERT_EQ(calledFromRender.end(), calledFromRender.find(state.tick()));
    calledFromTick.insert(state.tick());
    calledFromRender.insert(state.frame());
  }

  bool updateCalled = false;
  std::set<std::size_t> calledFromTick;
  std::set<std::size_t> calledFromRender;
};

class MockIntervalComponent : public IntervalComponent {
 public:
  explicit MockIntervalComponent(time_t interval = 1,
                                 bool isOneshot = false) noexcept
      : IntervalComponent(interval, isOneshot) {}

  void update(const EngineState& state) noexcept override {
    updateCalled = true;
    ASSERT_EQ(calledFromTick.end(), calledFromTick.find(state.tick()));
    ASSERT_EQ(calledFromRender.end(), calledFromRender.find(state.tick()));
    calledFromTick.insert(state.tick());
    calledFromRender.insert(state.frame());
  }

  bool updateCalled = false;
  std::set<std::size_t> calledFromTick;
  std::set<std::size_t> calledFromRender;
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_MOCK_MOCK_COMPONENT_H_
