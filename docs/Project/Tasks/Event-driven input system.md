---
tags: [task, input]
feature: "[[Features/Input system]]"
created: 2026-03-01
updated: 2026-03-01
status: planned
priority: medium
estimate: large
---

# Task: Event-driven Input System

## Goal

Create an event-driven `InputSystem` that allows subscription to specific key/mouse button combinations with exact modifier matching, complementing the existing polling-based `Input` state class.

## Context

The current `Input` class provides polling-based input queries (e.g., `isKeyDown(KEY_W)`), which works well for continuous state checking. However, many use cases benefit from event-driven callbacks that fire when specific input combinations occur. This task implements a subscription system where users can register callbacks for specific key+action+modifier combinations.

This follows the existing `System` architecture pattern and uses bitfield packing to create compact subscription tokens via the `|` operator.

## Acceptance Criteria

- [ ] Input enums (Key, Action, Mod, MouseButton) defined with non-overlapping bit ranges
- [ ] Bitwise OR operator overloads implemented for enum combinations
- [ ] `InputSystem` class created inheriting from `System` base class
- [ ] Keyboard subscription API: `subscribeKey(token, callback)` returns handle
- [ ] Mouse button subscription API: `subscribeMouse(token, callback)` returns handle
- [ ] Mouse movement subscription API: `subscribeMouseMove(callback)` returns handle
- [ ] Mouse scroll subscription API: `subscribeMouseScroll(callback)` returns handle
- [ ] Unsubscribe API: `unsubscribe(handle)` removes subscription
- [ ] Modifier matching is exact (all specified modifiers must be pressed, no extras)
- [ ] System integrates with existing `Input` class lifecycle
- [ ] Unit tests for subscription/unsubscribe/dispatch logic
- [ ] Example usage documented

## Implementation Notes

### Bitfield Layout (32-bit)

```
Bits 0-15  (16 bits): Key codes (0-65535)
Bits 16-19 (4 bits):  Action codes (0-15)
Bits 20-27 (8 bits):  Modifier flags
Bits 28-31 (4 bits):  Reserved
```

**Bit Ranges:**

- Keys: `0x0000_0000` to `0x0000_FFFF` (bits 0-15)
- Actions: `0x0001_0000` to `0x000F_0000` (bits 16-19)
- Modifiers: `0x0010_0000` to `0x0F00_0000` (bits 20-27)

**Example Enums:**

```cpp
// Keys enum (bits 0-15) - strongly typed, can use ASCII/GLFW-compatible values
enum class Key : u32 {
  Space = 0x0020,
  Apostrophe = 0x0027,
  Comma = 0x002C,
  Minus = 0x002D,
  Period = 0x002E,
  Slash = 0x002F,
  Num0 = 0x0030,
  // ... other number keys
  Num9 = 0x0039,
  Semicolon = 0x003B,
  Equal = 0x003D,
  A = 0x0041,
  B = 0x0042,
  // ... rest of alphabet
  Z = 0x005A,
  LeftBracket = 0x005B,
  Backslash = 0x005C,
  RightBracket = 0x005D,
  GraveAccent = 0x0060,
  
  // Function and special keys
  Escape = 0x0100,
  Enter = 0x0101,
  Tab = 0x0102,
  Backspace = 0x0103,
  Insert = 0x0104,
  Delete = 0x0105,
  Right = 0x0106,
  Left = 0x0107,
  Down = 0x0108,
  Up = 0x0109,
  PageUp = 0x010A,
  PageDown = 0x010B,
  Home = 0x010C,
  End = 0x010D,
  CapsLock = 0x0118,
  ScrollLock = 0x0119,
  NumLock = 0x011A,
  PrintScreen = 0x011B,
  Pause = 0x011C,
  F1 = 0x0122,
  F2 = 0x0123,
  // ... F3-F24
  F25 = 0x013A,
  
  // Keypad keys
  Kp0 = 0x0140,
  // ... Kp1-Kp9
  KpDecimal = 0x014A,
  KpDivide = 0x014B,
  KpMultiply = 0x014C,
  KpSubtract = 0x014D,
  KpAdd = 0x014E,
  KpEnter = 0x014F,
  KpEqual = 0x0150,
  
  // Modifier keys
  LeftShift = 0x0154,
  LeftControl = 0x0155,
  LeftAlt = 0x0156,
  LeftSuper = 0x0157,
  RightShift = 0x0158,
  RightControl = 0x0159,
  RightAlt = 0x015A,
  RightSuper = 0x015B,
  Menu = 0x015C,
};

// Actions enum (bits 16-19) - strongly typed
enum class Action : u32 {
  Press   = 0x00010000,  // bit 16
  Release = 0x00020000,  // bit 17
  Repeat  = 0x00040000,  // bit 18
};

// Modifiers enum (bits 20-27) - flag enum for bitwise operations
enum class Mod : u32 {
  None     = 0x00000000,
  Shift    = 0x00100000,  // bit 20
  Control  = 0x00200000,  // bit 21
  Alt      = 0x00400000,  // bit 22
  Super    = 0x00800000,  // bit 23
  CapsLock = 0x01000000,  // bit 24
  NumLock  = 0x02000000,  // bit 25
};

// Mouse buttons enum - similar pattern to Key
enum class MouseButton : u32 {
  Button1 = 0x0001,
  Button2 = 0x0002,
  Button3 = 0x0003,
  Button4 = 0x0004,
  Button5 = 0x0005,
  Button6 = 0x0006,
  Button7 = 0x0007,
  Button8 = 0x0008,
  Left   = Button1,
  Right  = Button2,
  Middle = Button3,
};

// Bitwise OR operator overloads for fluent API
constexpr u32 operator|(Key key, Action action) noexcept {
  return static_cast<u32>(key) | static_cast<u32>(action);
}

constexpr u32 operator|(u32 token, Mod mod) noexcept {
  return token | static_cast<u32>(mod);
}

constexpr u32 operator|(Mod lhs, Mod rhs) noexcept {
  return static_cast<u32>(lhs) | static_cast<u32>(rhs);
}

constexpr u32 operator|(MouseButton button, Action action) noexcept {
  return static_cast<u32>(button) | static_cast<u32>(action);
}
```

### API Design

**Keyboard Subscriptions:**

```cpp
// Subscribe to key combinations using enum values
auto h1 = inputSystem.subscribeKey(Key::W | Action::Press, callback);
auto h2 = inputSystem.subscribeKey(Key::S | Action::Press | Mod::Shift, callback);
auto h3 = inputSystem.subscribeKey(Key::Q | Action::Release | Mod::Control | Mod::Shift, callback);

// Callback signature
void keyCallback(Key key, Action action, Mod mods);
```

**Mouse Button Subscriptions:**

```cpp
// Subscribe to mouse button combinations using enum values
auto h1 = inputSystem.subscribeMouse(MouseButton::Left | Action::Press, callback);
auto h2 = inputSystem.subscribeMouse(MouseButton::Right | Action::Press | Mod::Alt, callback);

// Callback signature
void mouseButtonCallback(MouseButton button, Action action, Mod mods);
```

**Mouse Movement/Scroll:**

```cpp
// These don't use the bitfield token system
auto h1 = inputSystem.subscribeMouseMove(callback);  // void callback(f32 x, y, dx, dy)
auto h2 = inputSystem.subscribeMouseScroll(callback);  // void callback(f32 dx, dy)
```

**Unsubscribe:**

```cpp
inputSystem.unsubscribe(handle);
```

### Subscription Handle

Return an opaque handle type that can be used to unsubscribe:

```cpp
struct SubscriptionHandle {
  u64 id;  // Unique identifier
  bool operator==(const SubscriptionHandle&) const = default;
};
```

### InputSystem Class Structure

```cpp
class InputSystem : public System {
 public:
  InputSystem(Input* input);
  
  SubscriptionHandle subscribeKey(u32 token, std::function<void(Key, Action, Mod)> callback);
  SubscriptionHandle subscribeMouse(u32 token, std::function<void(MouseButton, Action, Mod)> callback);
  SubscriptionHandle subscribeMouseMove(std::function<void(f32, f32, f32, f32)> callback);
  SubscriptionHandle subscribeMouseScroll(std::function<void(f32, f32)> callback);
  
  void unsubscribe(SubscriptionHandle handle);
  
  // System lifecycle hooks
  void onTick(time_t delta) noexcept override;
  
 private:
  Input* input_;
  u64 nextHandleId_{0};
  
  // Storage for subscriptions
  std::unordered_map<u32, std::vector<std::pair<SubscriptionHandle, std::function<void(Key, Action, Mod)>>>> keySubscriptions_;
  std::unordered_map<u32, std::vector<std::pair<SubscriptionHandle, std::function<void(MouseButton, Action, Mod)>>>> mouseSubscriptions_;
  std::vector<std::pair<SubscriptionHandle, std::function<void(f32, f32, f32, f32)>>> mouseMoveSubscriptions_;
  std::vector<std::pair<SubscriptionHandle, std::function<void(f32, f32)>>> mouseScrollSubscriptions_;
};
```

### Integration Points

- `InputSystem` should be added to the engine's `SystemManager`
- During `onTick()`, it checks the `Input` state and dispatches to subscribers
- The platform layer (GLFW) continues to update the `Input` state as it does now
- `InputSystem` reads from `Input` and dispatches events to subscribers

**State Change Detection:**

- `InputSystem` stores previous frame state internally
- Each frame in `onTick()`, it compares current `Input` state with previous state
- When a key/button state transition is detected (e.g., not pressed → pressed), it dispatches to matching subscribers
- Mouse move/scroll events are dispatched every frame when delta values are non-zero

### Design Considerations

1. **Modifier Exact Match**: When subscribing with `Key::W | Mod::Shift`, it should only trigger when exactly Shift+W is pressed (not Shift+Ctrl+W). Extract and compare the modifier bits. Special case: `Mod::None` means "no modifiers pressed" (exact match to zero modifiers).

2. **Action Matching**: Support multiple actions in a single subscription. Use bitwise AND to check if the current action matches any subscribed action:

   ```cpp
   // Token has Action::Press | Action::Release (bits 16 and 17 set)
   // Current event is Action::Press (bit 16 set)
   // Check: (currentAction & subscribedActions) != 0 → match!
   if ((static_cast<u32>(currentAction) & extractAction(token)) != 0) { /* dispatch */ }
   ```

3. **Token Extraction**: Helper functions to extract components from token:

   ```cpp
   inline Key extractKey(u32 token) { return static_cast<Key>(token & 0xFFFF); }
   inline Action extractAction(u32 token) { return static_cast<Action>(token & 0x000F0000); }
   inline Mod extractMods(u32 token) { return static_cast<Mod>(token & 0x0FF00000); }
   ```

4. **Performance**: Use `std::unordered_map` with token as key for O(1) lookup. Each token maps to a vector of callbacks.

5. **MouseButton Namespace**: `MouseButton` and `Key` enums both use bits 0-15, but they're kept separate via distinct subscription APIs (`subscribeKey` vs `subscribeMouse`). This is intentional and prevents accidental mixing of keyboard and mouse tokens.

6. **Remove cursorWorldPoint**: As discussed, remove `cursorWorldPoint()` functionality from the `Input` class since it's spatial logic that doesn't belong in raw input handling.

### Testing

- Test bitfield packing/unpacking with various key/action/mod combinations
- Test exact modifier matching (reject when extra modifiers pressed)
- Test `Mod::None` only triggers when no modifiers are pressed
- Test multiple action subscriptions (e.g., `Action::Press | Action::Release`)
- Test subscribe/unsubscribe lifecycle
- Test multiple callbacks on same token
- Test mouse move/scroll dispatch
- Test that unsubscribe prevents future callbacks
- Test MouseButton and Key token separation (no cross-contamination)
