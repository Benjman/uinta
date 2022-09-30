#include <algorithm>
#include <uinta/input/fwd.hpp>
#include <uinta/input/state.hpp>

namespace uinta {

inline bool hasAnyInput(const InputState& input) {
  return isAnyKeyDown(input) || isAnyMouseBonttonDown(input) || isMouseScrolled(input) || isMouseMoved(input);
}

inline bool isAltDown(const InputState& input) {
  return isAnyKeyDown(input) && input.io_flags & MOD_ALT;
}

inline bool isCtrlDown(const InputState& input) {
  return isAnyKeyDown(input) && input.io_flags & MOD_CONTROL;
}

inline bool isKeyDown(const InputState& input, input_key_t key) {
  return isAnyKeyDown(input) && std::find(input.keys_down.begin(), input.keys_down.end(), key) != input.keys_down.end();
}

inline bool isKeyPressed(const InputState& input, input_key_t key) {
  return isFlagSet(input::HAS_KEY_PRESSED, input.flags) &&
         std::find(input.keys_pressed.begin(), input.keys_pressed.end(), key) != input.keys_pressed.end();
}

inline bool isKeyReleased(const InputState& input, input_key_t key) {
  return std::find(input.keys_released.begin(), input.keys_released.end(), key) != input.keys_released.end();
}

inline bool isKeyRepeated(const InputState& input, input_key_t key) {
  return std::find(input.keys_repeated.begin(), input.keys_repeated.end(), key) != input.keys_repeated.end();
}

inline bool isMouseButtonDown(const InputState& input, mouse_button_t button) {
  return isFlagSet(input::HAS_MOUSE_DOWN, input.flags) &&
         std::find(input.mouse_down.begin(), input.mouse_down.end(), button) != input.mouse_down.end();
}

inline bool isMouseButtonPressed(const InputState& input, mouse_button_t button) {
  return isFlagSet(input::HAS_MOUSE_DOWN, input.flags) &&
         std::find(input.mouse_pressed.begin(), input.mouse_pressed.end(), button) != input.mouse_pressed.end();
}

inline bool isMouseButtonReleased(const InputState& input, mouse_button_t button) {
  return isFlagSet(input::HAS_KEY_RELEASED, input.flags) &&
         std::find(input.mouse_released.begin(), input.mouse_released.end(), button) != input.mouse_released.end();
}

inline bool isMouseScrolled(const InputState& input) {
  return isFlagSet(input::HAS_MOUSE_SCROLL, input.flags);
}

inline bool isMouseMoved(const InputState& input) {
  return isFlagSet(input::HAS_MOUSE_MOVE, input.flags);
}

inline bool isShiftDown(const InputState& input) {
  return isFlagSet(input::HAS_KEY_DOWN, input.flags) && input.io_flags & MOD_SHIFT;
}

inline void keyPressed(InputState& input, const input_key_t key, const int flags) {
  input.io_flags = flags;
  input.keys_pressed.insert(key);
  input.keys_down.insert(key);
  setFlag(input::HAS_KEY_PRESSED, true, input.flags);
  setFlag(input::HAS_KEY_DOWN, true, input.flags);
}

inline void keyReleased(InputState& input, const input_key_t key, const int flags) {
  input.io_flags = flags;
  input.keys_released.insert(key);
  input.keys_down.erase(key);
  input.keys_repeated.erase(key);
  setFlag(input::HAS_KEY_RELEASED, input.keys_released.size() > 0, input.flags);
  setFlag(input::HAS_KEY_DOWN, input.keys_down.size() > 0, input.flags);
  setFlag(input::HAS_KEY_REPEATED, input.keys_repeated.size() > 0, input.flags);
}

inline void keyRepeated(InputState& input, const input_key_t key, const int flags) {
  input.io_flags = flags;
  input.keys_repeated.insert(key);
  setFlag(input::HAS_KEY_REPEATED, true, input.flags);
}

inline void mouseButtonPressed(InputState& input, const input_key_t key, const int flags) {
  input.io_flags = flags;
  input.mouse_pressed.insert(key);
  input.mouse_down.insert(key);
  setFlag(input::HAS_MOUSE_PRESSED, true, input.flags);
  setFlag(input::HAS_MOUSE_DOWN, true, input.flags);
}

inline void mouseButtonReleased(InputState& input, const input_key_t key, const int flags) {
  input.io_flags = flags;
  input.mouse_released.insert(key);
  input.mouse_down.erase(key);
  setFlag(input::HAS_MOUSE_RELEASED, input.mouse_released.size() > 0, input.flags);
  setFlag(input::HAS_MOUSE_DOWN, input.mouse_down.size() > 0, input.flags);
}

inline void mouseMoved(InputState& input, const double xpos, const double ypos) {
  input.cursordx = xpos - input.cursorx;
  input.cursordy = ypos - input.cursory;
  input.cursorx = xpos;
  input.cursory = ypos;
  setFlag(input::HAS_MOUSE_MOVE, true, input.flags);
}

inline void mouseScrolled(InputState& input, const double dx, const double dy) {
  input.scrolldx = dx;
  input.scrolldy = dy;
  setFlag(input::HAS_MOUSE_SCROLL, true, input.flags);
}

inline void reset(InputState& input) {
  if (isFlagSet(input::HAS_KEY_PRESSED, input.flags)) {
    setFlag(input::HAS_KEY_PRESSED, false, input.flags);
    input.keys_pressed.clear();
  }
  if (isFlagSet(input::HAS_KEY_RELEASED, input.flags)) {
    setFlag(input::HAS_KEY_RELEASED, false, input.flags);
    input.keys_released.clear();
  }
  if (isFlagSet(input::HAS_MOUSE_PRESSED, input.flags)) {
    setFlag(input::HAS_MOUSE_PRESSED, false, input.flags);
    input.mouse_pressed.clear();
  }
  if (isFlagSet(input::HAS_MOUSE_RELEASED, input.flags)) {
    setFlag(input::HAS_MOUSE_RELEASED, false, input.flags);
    input.mouse_released.clear();
  }

  if (isFlagSet(input::HAS_MOUSE_MOVE, input.flags)) setFlag(input::HAS_MOUSE_MOVE, false, input.flags);
  if (isFlagSet(input::HAS_MOUSE_SCROLL, input.flags)) setFlag(input::HAS_MOUSE_SCROLL, false, input.flags);

  input.cursordx = 0;
  input.cursordy = 0;
  input.scrolldx = 0;
  input.scrolldy = 0;
}

}  // namespace uinta
