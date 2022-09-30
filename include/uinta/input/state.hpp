#ifndef UINTA_INPUT_STATE_HPP
#define UINTA_INPUT_STATE_HPP

#include <uinta/input/keys.hpp>
#include <unordered_set>

namespace uinta {

namespace internal {
bool isFoundInSet(const uinta::input_key_t key, const std::unordered_set<uinta::input_key_t>& set);
}

struct InputState final {
  int flags = 0;
  float cursorx, cursory;
  float cursordx, cursordy;
  float scrolldx, scrolldy;
  std::unordered_set<input_key_t> keys_down;
  std::unordered_set<input_key_t> keys_pressed;
  std::unordered_set<input_key_t> keys_released;
  std::unordered_set<input_key_t> keys_repeated;
  std::unordered_set<mouse_button_t> mouse_down;
  std::unordered_set<mouse_button_t> mouse_pressed;
  std::unordered_set<mouse_button_t> mouse_released;
};

inline bool isAltDown(const InputState& input) {
  return input.flags & MOD_ALT;
}

inline bool isAnyKeyDown(const InputState& input) {
  return input.keys_down.size();
}

inline bool isAnyMouseBonttonDown(const InputState& input) {
  return input.mouse_down.size();
}

inline bool isCtrlDown(const InputState& input) {
  return input.flags & MOD_CONTROL;
}

inline bool isKeyDown(const InputState& input, input_key_t key) {
  return internal::isFoundInSet(key, input.keys_down);
}

inline bool isKeyPressed(const InputState& input, input_key_t key) {
  return internal::isFoundInSet(key, input.keys_pressed);
}

inline bool isKeyReleased(const InputState& input, input_key_t key) {
  return internal::isFoundInSet(key, input.keys_released);
}

inline bool isKeyRepeated(const InputState& input, input_key_t key) {
  return internal::isFoundInSet(key, input.keys_repeated);
}

inline bool isMouseButtonDown(const InputState& input, mouse_button_t button) {
  return internal::isFoundInSet(button, input.mouse_down);
}

inline bool isMouseButtonPressed(const InputState& input, mouse_button_t button) {
  return internal::isFoundInSet(button, input.mouse_pressed);
}

inline bool isMouseButtonReleased(const InputState& input, mouse_button_t button) {
  return internal::isFoundInSet(button, input.mouse_released);
}

inline bool isShiftDown(const InputState& input) {
  return input.flags & MOD_SHIFT;
}

inline void keyPressed(InputState& input, const input_key_t key, const int flags) {
  input.flags = flags;
  input.keys_pressed.insert(key);
  input.keys_down.insert(key);
}

inline void keyReleased(InputState& input, const input_key_t key, const int flags) {
  input.flags = flags;
  input.keys_released.insert(key);
  input.keys_down.erase(key);
}

inline void keyRepeated(InputState& input, const input_key_t key, const int flags) {
  input.flags = flags;
  input.keys_repeated.insert(key);
}

inline void mouseButtonPressed(InputState& input, const input_key_t key, const int flags) {
  input.flags = flags;
  input.mouse_pressed.insert(key);
  input.mouse_down.insert(key);
}

inline void mouseButtonReleased(InputState& input, const input_key_t key, const int flags) {
  input.flags = flags;
  input.mouse_released.insert(key);
  input.mouse_down.erase(key);
}

inline void reset(InputState& input) {
  input.cursordx = 0;
  input.cursordy = 0;
  input.scrolldx = 0;
  input.scrolldy = 0;
  input.keys_pressed.clear();
  input.keys_released.clear();
  input.keys_repeated.clear();
  input.mouse_pressed.clear();
  input.mouse_released.clear();
}

}  // namespace uinta

#endif  // UINTA_INPUT_STATE_HPP
