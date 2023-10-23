#ifndef UINTA_FWD_HPP
#define UINTA_FWD_HPP

#include <uinta/types.h>
namespace uinta {
class FileManager;
class GridScene;
class Runner;
class Scene;
class TargetCamera;
class UiElement;
class UiManager;
class UiPane;
class UintaException;
struct Event;
struct InputState;
struct Light;
struct RunnerState;
struct Window;
using event_t = u32;
using input_key_t = u32;
using mouse_button_t = u32;
using subscription_t = u32;
using uinta_error_code_t = i32;
}  // namespace uinta

#include <system_error>
namespace uinta {
using uinta_error_code = std::error_code;
}

#include <uinta/flags.h>
#include <uinta/utils/macros.h>

#endif  // UINTA_FWD_HPP
