#ifndef UINTA_INPUT_FWD_HPP
#define UINTA_INPUT_FWD_HPP

#include <uinta/flags.hpp>

namespace uinta {
using input_key_t = unsigned int;
using mouse_button_t = unsigned int;

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

}  // namespace input
}  // namespace uinta

#endif  // UINTA_INPUT_FWD_HPP
