#ifndef UINTA_INPUT_FWD_HPP
#define UINTA_INPUT_FWD_HPP

#include <uinta/flags.hpp>

namespace uinta {
struct InputState;

namespace input {

static const flag_t HAS_KEY_DOWN = 0x1;
static const flag_t HAS_KEY_PRESSED = 0x2;
static const flag_t HAS_KEY_RELEASED = 0x4;
static const flag_t HAS_KEY_REPEATED = 0x8;
static const flag_t HAS_MOUSE_DOWN = 0x10;
static const flag_t HAS_MOUSE_MOVE = 0x20;
static const flag_t HAS_MOUSE_PRESSED = 0x40;
static const flag_t HAS_MOUSE_RELEASED = 0x80;
static const flag_t HAS_MOUSE_SCROLL = 0x100;

bool isAnyKeyDown(const InputState& input);
bool isAnyMouseBonttonDown(const InputState& input);
bool isKeyDown(const InputState& input);
bool isKeyPressed(const InputState& input);
bool isKeyReleased(const InputState& input);
bool isKeyRepeated(const InputState& input);
bool isMouseDown(const InputState& input);
bool isMouseMove(const InputState& input);
bool isMousePressed(const InputState& input);
bool isMouseReleased(const InputState& input);
bool isMouseScroll(const InputState& input);

}  // namespace input
}  // namespace uinta

#endif  // UINTA_INPUT_FWD_HPP
