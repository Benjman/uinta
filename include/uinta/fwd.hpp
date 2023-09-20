#ifndef UINTA_FWD_HPP
#define UINTA_FWD_HPP

#include <uinta/flags.h>
#include <uinta/types.h>
#include <uinta/utils/macros.h>

#include <system_error>

namespace uinta {

class FileManager;
class Grid;
class Runner;
class Scene;
class UintaException;
struct Light;
struct RunnerState;
struct Window;
struct InputState;

using input_key_t = u32;
using mouse_button_t = u32;
using uinta_error_code = std::error_code;
using uinta_error_code_t = i32;

}  // namespace uinta

#endif  // UINTA_FWD_HPP
