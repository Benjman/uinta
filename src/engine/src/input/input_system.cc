#include "uinta/input/input_system.h"

#include <algorithm>

#include "uinta/input/fwd.h"

namespace uinta {

InputSystem::InputSystem(Input* input) noexcept : input_(input) {
  // Initialize previous state arrays to unknown
  prevKeysDown_.fill(_::InputUnknown);
  prevMouseDown_.fill(_::InputUnknown);
}

SubscriptionHandle InputSystem::subscribeKey(u32 token,
                                             KeyCallback callback) noexcept {
  auto handle = generateHandle();
  keySubscriptions_[token].push_back({handle, std::move(callback)});
  return handle;
}

SubscriptionHandle InputSystem::subscribeMouse(
    u32 token, MouseButtonCallback callback) noexcept {
  auto handle = generateHandle();
  mouseSubscriptions_[token].push_back({handle, std::move(callback)});
  return handle;
}

SubscriptionHandle InputSystem::subscribeMouseMove(
    MouseMoveCallback callback) noexcept {
  auto handle = generateHandle();
  mouseMoveSubscriptions_.push_back({handle, std::move(callback)});
  return handle;
}

SubscriptionHandle InputSystem::subscribeMouseScroll(
    MouseScrollCallback callback) noexcept {
  auto handle = generateHandle();
  mouseScrollSubscriptions_.push_back({handle, std::move(callback)});
  return handle;
}

void InputSystem::unsubscribe(SubscriptionHandle handle) noexcept {
  // Search key subscriptions
  for (auto& [token, subscriptions] : keySubscriptions_) {
    auto it = std::find_if(
        subscriptions.begin(), subscriptions.end(),
        [&handle](const KeySubscription& sub) { return sub.handle == handle; });
    if (it != subscriptions.end()) {
      subscriptions.erase(it);
      // Clean up empty vectors
      if (subscriptions.empty()) {
        keySubscriptions_.erase(token);
      }
      return;
    }
  }

  // Search mouse button subscriptions
  for (auto& [token, subscriptions] : mouseSubscriptions_) {
    auto it = std::find_if(subscriptions.begin(), subscriptions.end(),
                           [&handle](const MouseButtonSubscription& sub) {
                             return sub.handle == handle;
                           });
    if (it != subscriptions.end()) {
      subscriptions.erase(it);
      if (subscriptions.empty()) {
        mouseSubscriptions_.erase(token);
      }
      return;
    }
  }

  // Search mouse move subscriptions
  auto moveIt = std::find_if(mouseMoveSubscriptions_.begin(),
                             mouseMoveSubscriptions_.end(),
                             [&handle](const MouseMoveSubscription& sub) {
                               return sub.handle == handle;
                             });
  if (moveIt != mouseMoveSubscriptions_.end()) {
    mouseMoveSubscriptions_.erase(moveIt);
    return;
  }

  // Search mouse scroll subscriptions
  auto scrollIt = std::find_if(mouseScrollSubscriptions_.begin(),
                               mouseScrollSubscriptions_.end(),
                               [&handle](const MouseScrollSubscription& sub) {
                                 return sub.handle == handle;
                               });
  if (scrollIt != mouseScrollSubscriptions_.end()) {
    mouseScrollSubscriptions_.erase(scrollIt);
    return;
  }
}

void InputSystem::update(time_t) noexcept {
  processKeyboardEvents();
  processMouseButtonEvents();
  processMouseMoveEvents();
  processMouseScrollEvents();
}

SubscriptionHandle InputSystem::generateHandle() noexcept {
  return SubscriptionHandle{nextHandleId_++};
}

bool InputSystem::matchesAction(Action currentAction,
                                Action subscribedActions) const noexcept {
  return (static_cast<u32>(currentAction) &
          static_cast<u32>(subscribedActions)) != 0;
}

bool InputSystem::matchesModifiers(Mod currentMods,
                                   Mod subscribedMods) const noexcept {
  return currentMods == subscribedMods;
}

void InputSystem::processKeyboardEvents() noexcept {
  if (!input_) {
    return;
  }

  // Convert current modifier state to our Mod enum
  Mod currentMods = toMod(input_->mods());

  // Process key press events
  if (input_->anyKeyPressed()) {
    const auto& keysPressed = input_->keysPressed();
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      InputKey inputKey = keysPressed[i];
      if (inputKey == _::InputUnknown) {
        break;
      }

      Key key = toKey(inputKey);
      Action action = Action::Press;

      // Iterate through all subscriptions to find matches
      // We need to check each token because subscribers may have different
      // modifier combinations or multiple actions
      for (auto& [token, subscriptions] : keySubscriptions_) {
        Key tokenKey = extractKey(token);
        if (tokenKey != key) {
          continue;
        }

        Action tokenActions = extractAction(token);
        if (!matchesAction(action, tokenActions)) {
          continue;
        }

        Mod tokenMods = extractMods(token);
        if (!matchesModifiers(currentMods, tokenMods)) {
          continue;
        }

        // Dispatch to all matching subscribers for this token
        for (auto& sub : subscriptions) {
          sub.callback(key, action, currentMods);
        }
      }
    }
  }

  // Process key release events
  if (input_->anyKeyReleased()) {
    const auto& keysReleased = input_->keysReleased();
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      InputKey inputKey = keysReleased[i];
      if (inputKey == _::InputUnknown) {
        break;
      }

      Key key = toKey(inputKey);
      Action action = Action::Release;

      for (auto& [token, subscriptions] : keySubscriptions_) {
        Key tokenKey = extractKey(token);
        if (tokenKey != key) {
          continue;
        }

        Action tokenActions = extractAction(token);
        if (!matchesAction(action, tokenActions)) {
          continue;
        }

        Mod tokenMods = extractMods(token);
        if (!matchesModifiers(currentMods, tokenMods)) {
          continue;
        }

        for (auto& sub : subscriptions) {
          sub.callback(key, action, currentMods);
        }
      }
    }
  }

  // Process key repeat events
  if (input_->anyKeyRepeated()) {
    const auto& keysRepeated = input_->keysRepeated();
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      InputKey inputKey = keysRepeated[i];
      if (inputKey == _::InputUnknown) {
        break;
      }

      Key key = toKey(inputKey);
      Action action = Action::Repeat;

      for (auto& [token, subscriptions] : keySubscriptions_) {
        Key tokenKey = extractKey(token);
        if (tokenKey != key) {
          continue;
        }

        Action tokenActions = extractAction(token);
        if (!matchesAction(action, tokenActions)) {
          continue;
        }

        Mod tokenMods = extractMods(token);
        if (!matchesModifiers(currentMods, tokenMods)) {
          continue;
        }

        for (auto& sub : subscriptions) {
          sub.callback(key, action, currentMods);
        }
      }
    }
  }

  // Update previous state
  prevKeysDown_ = input_->keysDown();
}

void InputSystem::processMouseButtonEvents() noexcept {
  if (!input_) {
    return;
  }

  Mod currentMods = toMod(input_->mods());

  // Process mouse press events
  if (input_->anyMousePressed()) {
    const auto& mousePressed = input_->mousePressed();
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      MouseButton inputButton = mousePressed[i];
      if (inputButton == _::InputUnknown) {
        break;
      }

      MouseBtn button = toMouseButton(inputButton);
      Action action = Action::Press;

      for (auto& [token, subscriptions] : mouseSubscriptions_) {
        MouseBtn tokenButton = extractMouseButton(token);
        if (tokenButton != button) {
          continue;
        }

        Action tokenActions = extractAction(token);
        if (!matchesAction(action, tokenActions)) {
          continue;
        }

        Mod tokenMods = extractMods(token);
        if (!matchesModifiers(currentMods, tokenMods)) {
          continue;
        }

        for (auto& sub : subscriptions) {
          sub.callback(button, action, currentMods);
        }
      }
    }
  }

  // Process mouse release events
  if (input_->anyMouseReleased()) {
    const auto& mouseReleased = input_->mouseReleased();
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      MouseButton inputButton = mouseReleased[i];
      if (inputButton == _::InputUnknown) {
        break;
      }

      MouseBtn button = toMouseButton(inputButton);
      Action action = Action::Release;

      for (auto& [token, subscriptions] : mouseSubscriptions_) {
        MouseBtn tokenButton = extractMouseButton(token);
        if (tokenButton != button) {
          continue;
        }

        Action tokenActions = extractAction(token);
        if (!matchesAction(action, tokenActions)) {
          continue;
        }

        Mod tokenMods = extractMods(token);
        if (!matchesModifiers(currentMods, tokenMods)) {
          continue;
        }

        for (auto& sub : subscriptions) {
          sub.callback(button, action, currentMods);
        }
      }
    }
  }

  // Update previous state
  prevMouseDown_ = input_->mouseDown();
}

void InputSystem::processMouseMoveEvents() noexcept {
  if (!input_) {
    return;
  }

  if (input_->mouseMoved()) {
    f32 x = input_->cursorx();
    f32 y = input_->cursory();
    f32 dx = input_->cursordx();
    f32 dy = input_->cursordy();

    for (auto& sub : mouseMoveSubscriptions_) {
      sub.callback(x, y, dx, dy);
    }
  }
}

void InputSystem::processMouseScrollEvents() noexcept {
  if (!input_) {
    return;
  }

  if (input_->mouseScrolled()) {
    f32 dx = input_->scrolldx();
    f32 dy = input_->scrolldy();

    for (auto& sub : mouseScrollSubscriptions_) {
      sub.callback(dx, dy);
    }
  }
}

}  // namespace uinta