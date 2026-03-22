#ifndef SRC_ENGINE_INCLUDE_UINTA_INPUT_INPUT_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_INPUT_INPUT_SYSTEM_H_

#include <functional>
#include <unordered_map>
#include <vector>

#include "uinta/input.h"
#include "uinta/input/input_token.h"
#include "uinta/types.h"

namespace uinta {

struct KeyInfo {
  Key key;
  Action action;
  Mod mods;
};
struct MouseButtonInfo {
  MouseBtn button;
  Action action;
  Mod mods;
};
struct MouseMoveInfo {
  f32 x;
  f32 y;
  f32 dx;
  f32 dy;
};
struct MouseScrollInfo {
  f32 dx;
  f32 dy;
};

class InputSystem {
 public:
  using KeyCallback = std::function<void(const KeyInfo&)>;
  using MouseButtonCallback = std::function<void(const MouseButtonInfo&)>;
  using MouseMoveCallback = std::function<void(const MouseMoveInfo&)>;
  using MouseScrollCallback = std::function<void(const MouseScrollInfo&)>;

  InputSystem() noexcept;

  ~InputSystem() noexcept = default;

  InputSystem(const InputSystem&) noexcept = delete;
  InputSystem& operator=(const InputSystem&) noexcept = delete;
  InputSystem(InputSystem&&) noexcept = delete;
  InputSystem& operator=(InputSystem&&) noexcept = delete;

  SubscriptionHandle subscribeKey(u32 token, KeyCallback callback) noexcept;

  SubscriptionHandle subscribeMouse(u32 token, MouseButtonCallback callback) noexcept;

  SubscriptionHandle subscribeMouseMove(MouseMoveCallback callback) noexcept;

  SubscriptionHandle subscribeMouseScroll(MouseScrollCallback callback) noexcept;

  void unsubscribe(SubscriptionHandle handle) noexcept;

  void update(time_t /*unused*/) noexcept;

  void endFrame() noexcept { input_.reset(); }

  const Input* input() const noexcept { return &input_; }

  Input* input() noexcept { return &input_; }

 private:
  struct KeySubscription {
    SubscriptionHandle handle;
    KeyCallback callback;
  };

  struct MouseButtonSubscription {
    SubscriptionHandle handle;
    MouseButtonCallback callback;
  };

  struct MouseMoveSubscription {
    SubscriptionHandle handle;
    MouseMoveCallback callback;
  };

  struct MouseScrollSubscription {
    SubscriptionHandle handle;
    MouseScrollCallback callback;
  };

  SubscriptionHandle generateHandle() noexcept;

  bool matchesAction(Action currentAction, Action subscribedActions) const noexcept;

  bool matchesModifiers(Mod currentMods, Mod subscribedMods) const noexcept;

  void processKeyboardEvents() noexcept;

  void processMouseButtonEvents() noexcept;

  void processMouseMoveEvents() noexcept;

  void processMouseScrollEvents() noexcept;

  Input input_;
  u64 nextHandleId_{0};

  std::unordered_map<u32, std::vector<KeySubscription>> keySubscriptions_;
  std::unordered_map<u32, std::vector<MouseButtonSubscription>> mouseSubscriptions_;
  std::vector<MouseMoveSubscription> mouseMoveSubscriptions_;
  std::vector<MouseScrollSubscription> mouseScrollSubscriptions_;

  Input::KeyStorage prevKeysDown_;
  Input::MouseStorage prevMouseDown_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_INPUT_INPUT_SYSTEM_H_
