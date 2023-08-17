#ifndef UINTA_INPUT_FWD_HPP
#define UINTA_INPUT_FWD_HPP

#include <uinta/flags.h>

namespace uinta {

using input_key_t = u32;
using mouse_button_t = u32;

struct InputState;

inline bool isAnyKeyDown(const InputState& input);
inline bool isAnyKeyPressed(const InputState& input);
inline bool isAnyKeyReleased(const InputState& input);
inline bool isAnyKeyRepeated(const InputState& input);
inline bool isAnyMouseDown(const InputState& input);
inline bool isAnyMouseMove(const InputState& input);
inline bool isAnyMousePressed(const InputState& input);
inline bool isAnyMouseReleased(const InputState& input);

inline bool isAltDown(const InputState& input);
inline bool isCtrlDown(const InputState& input);
inline bool isKeyDown(const InputState& input, input_key_t key);
inline bool isKeyPressed(const InputState& input, input_key_t key);
inline bool isKeyReleased(const InputState& input, input_key_t key);
inline bool isKeyRepeated(const InputState& input, input_key_t key);
inline bool isMouseButtonDown(const InputState& input, mouse_button_t button);
inline bool isMouseButtonPressed(const InputState& input, mouse_button_t button);
inline bool isMouseButtonReleased(const InputState& input, mouse_button_t button);
inline bool isMouseReleased(const InputState& input, mouse_button_t button);

inline bool isMouseMoved(const InputState& input);
inline bool isMouseScroll(const InputState& input);
inline bool isMouseScrolled(const InputState& input);
inline bool isShiftDown(const InputState& input);

inline void keyPressed(InputState& input, const input_key_t key, const i32 flags);
inline void keyReleased(InputState& input, const input_key_t key, const i32 flags);
inline void keyRepeated(InputState& input, const input_key_t key, const i32 flags);

inline void mouseButtonPressed(InputState& input, const input_key_t key, const i32 flags);
inline void mouseButtonReleased(InputState& input, const input_key_t key, const i32 flags);
inline void mouseMoved(InputState& input, const f64 xpos, const f64 ypos);
inline void mouseScrolled(InputState& input, const f64 dx, const f64 dy);

inline void reset(InputState& input);
inline void resetKeyboard(InputState& input);
inline void resetMouse(InputState& input);

inline const char* const getKeyStr(input_key_t key) noexcept;
inline const char* const getActionStr(input_key_t action) noexcept;
inline const char* const getModsStr(input_key_t mods) noexcept;
inline const char* const getMouseButtonStr(mouse_button_t button) noexcept;

namespace input {

static const flag_t HAS_KEY_DOWN = 1 << 0;
static const flag_t HAS_KEY_PRESSED = 1 << 1;
static const flag_t HAS_KEY_RELEASED = 1 << 2;
static const flag_t HAS_KEY_REPEATED = 1 << 3;
static const flag_t HAS_MOUSE_DOWN = 1 << 4;
static const flag_t HAS_MOUSE_MOVE = 1 << 5;
static const flag_t HAS_MOUSE_PRESSED = 1 << 6;
static const flag_t HAS_MOUSE_RELEASED = 1 << 7;
static const flag_t HAS_MOUSE_SCROLL = 1 << 8;

}  // namespace input
}  // namespace uinta

#endif  // UINTA_INPUT_FWD_HPP
