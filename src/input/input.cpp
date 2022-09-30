#include <uinta/flags.hpp>
#include <uinta/input/fwd.hpp>
#include <uinta/input/state.hpp>

namespace uinta {
namespace input {

// clang-format off
inline bool isKeyDown(const InputState& input) { return isFlagSet(HAS_KEY_DOWN, input.flags); }
inline bool isKeyPressed(const InputState& input) { return isFlagSet(HAS_KEY_PRESSED, input.flags); }
inline bool isKeyReleased(const InputState& input) { return isFlagSet(HAS_KEY_RELEASED, input.flags); }
inline bool isKeyRepeated(const InputState& input) { return isFlagSet(HAS_KEY_REPEATED, input.flags); }
inline bool isMouseDown(const InputState& input) { return isFlagSet(HAS_MOUSE_DOWN, input.flags); }
inline bool isMouseMove(const InputState& input) { return isFlagSet(HAS_MOUSE_MOVE, input.flags); }
inline bool isMousePressed(const InputState& input) { return isFlagSet(HAS_MOUSE_PRESSED, input.flags); }
inline bool isMouseReleased(const InputState& input) { return isFlagSet(HAS_MOUSE_RELEASED, input.flags); }
inline bool isMouseScroll(const InputState& input) { return isFlagSet(HAS_MOUSE_SCROLL, input.flags); }
inline bool isAnyKeyDown(const InputState& input) { return isFlagSet(HAS_KEY_DOWN, input.flags); }
inline bool isAnyMouseBonttonDown(const InputState& input) { return isFlagSet(HAS_MOUSE_DOWN, input.flags); }
// clang-format on

}  // namespace input
}  // namespace uinta
