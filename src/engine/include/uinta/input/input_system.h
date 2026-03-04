#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_SYSTEM_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_SYSTEM_H_

#include <functional>
#include <unordered_map>
#include <vector>

#include "uinta/component.h"
#include "uinta/input/input.h"
#include "uinta/input/input_token.h"
#include "uinta/types.h"

namespace uinta {

// Event-driven input system that allows subscription to specific key/mouse
// button combinations with exact modifier matching.
//
// This complements the existing polling-based Input class by providing
// event-driven callbacks that fire when specific input combinations occur.
class InputSystem : public TickComponent {
 public:
  using KeyCallback = std::function<void(Key, Action, Mod)>;
  using MouseButtonCallback = std::function<void(MouseBtn, Action, Mod)>;
  using MouseMoveCallback = std::function<void(f32, f32, f32, f32)>;
  using MouseScrollCallback = std::function<void(f32, f32)>;

  explicit InputSystem(Input* input) noexcept;

  ~InputSystem() noexcept override = default;

  InputSystem(const InputSystem&) noexcept = delete;
  InputSystem& operator=(const InputSystem&) noexcept = delete;
  InputSystem(InputSystem&&) noexcept = delete;
  InputSystem& operator=(InputSystem&&) noexcept = delete;

  // Subscribe to keyboard input combinations.
  // Token format: Key | Action | Mod (optional)
  // Returns a handle that can be used to unsubscribe.
  //
  // Modifier matching is exact: if Mod::Shift is specified, the callback
  // will only fire when Shift is pressed and no other modifiers are active.
  // Use Mod::None (or omit modifiers) to match when no modifiers are pressed.
  //
  // Action matching uses bitwise OR: if multiple actions are specified in the
  // token (e.g., Action::Press | Action::Release), the callback will fire
  // for any of those actions.
  SubscriptionHandle subscribeKey(u32 token, KeyCallback callback) noexcept;

  // Subscribe to mouse button combinations.
  // Token format: MouseBtn | Action | Mod (optional)
  // Returns a handle that can be used to unsubscribe.
  //
  // Modifier and action matching work the same as subscribeKey.
  SubscriptionHandle subscribeMouse(u32 token,
                                    MouseButtonCallback callback) noexcept;

  // Subscribe to mouse movement events.
  // Callback receives: (x, y, dx, dy) where x/y are absolute position
  // and dx/dy are deltas since last frame.
  // Fires every frame when the mouse moves.
  SubscriptionHandle subscribeMouseMove(MouseMoveCallback callback) noexcept;

  // Subscribe to mouse scroll events.
  // Callback receives: (dx, dy) scroll deltas.
  // Fires every frame when scrolling occurs.
  SubscriptionHandle subscribeMouseScroll(
      MouseScrollCallback callback) noexcept;

  // Unsubscribe from input events.
  // The handle becomes invalid after this call.
  void unsubscribe(SubscriptionHandle handle) noexcept;

  // Component lifecycle hook - called every frame to process input and dispatch
  // to subscribers.
  void update(time_t) noexcept override;

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

  // Generate unique subscription handle ID
  SubscriptionHandle generateHandle() noexcept;

  // Check if action matches the subscribed actions (bitwise OR)
  bool matchesAction(Action currentAction,
                     Action subscribedActions) const noexcept;

  // Check if modifiers match exactly
  bool matchesModifiers(Mod currentMods, Mod subscribedMods) const noexcept;

  // Process keyboard state changes and dispatch to subscribers
  void processKeyboardEvents() noexcept;

  // Process mouse button state changes and dispatch to subscribers
  void processMouseButtonEvents() noexcept;

  // Process mouse movement and dispatch to subscribers
  void processMouseMoveEvents() noexcept;

  // Process mouse scroll and dispatch to subscribers
  void processMouseScrollEvents() noexcept;

  Input* input_;
  u64 nextHandleId_{0};

  // Storage for subscriptions
  // Key is the token (or base token without action bits for iteration)
  std::unordered_map<u32, std::vector<KeySubscription>> keySubscriptions_;
  std::unordered_map<u32, std::vector<MouseButtonSubscription>>
      mouseSubscriptions_;
  std::vector<MouseMoveSubscription> mouseMoveSubscriptions_;
  std::vector<MouseScrollSubscription> mouseScrollSubscriptions_;

  // Previous frame state for detecting transitions
  Input::KeyStorage prevKeysDown_;
  Input::MouseStorage prevMouseDown_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_SYSTEM_H_
