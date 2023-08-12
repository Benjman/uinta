#ifndef UINTA_GLFW_RUNNER_UI_HPP
#define UINTA_GLFW_RUNNER_UI_HPP

#include <uinta/fwd.hpp>

namespace uinta {

class GlfwRunner;

namespace ui {

static constexpr flag_t INPUT_HANDLED_MOUSE = 1 << 0;
static constexpr flag_t INPUT_HANDLED_KEYBOARD = 1 << 1;

static flags_t flags = 0;

void onInit(GlfwRunner& runner);
void onPreTick(GlfwRunner& runner, const RunnerState& state);
void onTick(GlfwRunner& runner, const RunnerState& state);
void onPostTick(GlfwRunner& runner, const RunnerState& state);
void onPreRender(GlfwRunner& runner, const RunnerState& state);
void onRender(GlfwRunner& runner, const RunnerState& state);
void onPostRender(GlfwRunner& runner, const RunnerState& state);
void onShutdown(GlfwRunner& runner);

}  // namespace ui
}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_UI_HPP
