#ifndef UINTA_INPUT_STATE_HPP
#define UINTA_INPUT_STATE_HPP

#include <uinta/input/keys.hpp>
#include <unordered_set>

namespace uinta {

struct InputState final {
  int platform_flags = 0;
  flags_t flags = 0;
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

}  // namespace uinta

#endif  // UINTA_INPUT_STATE_HPP
