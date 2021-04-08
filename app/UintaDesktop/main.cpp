#include <uinta/gl/gl_error.h>
#include "glfw.h"

#include <uinta/controller/scene/scene_controller.h>
#include <uinta/debug/debug_ui_controller.h>
#include <uinta/engine_state.h>
#include <uinta/input/input_manager.h>
#include <uinta/shader.h>
#include <uinta/text.h>

using namespace uinta;
using namespace uinta::glfw;

class MainController : public Controller, public IRenderController {
	DebugUiController debugUi = DebugUiController(this);
	SceneController scene = SceneController(this);

public:
	MainController() : Controller(nullptr) {
	}

	void render() override {
		scene.render();
		debugUi.render();
	}

	void initialize() override {
		glEnable(GL_DEPTH_TEST);
		glCheckError(GL_ENABLE);
	}

};

int main() {
	GlfwDto glfw(1282, 532, "Test Window Creation");
//	glfw.setHeadless(true);

	if (!initialize(glfw) || glfw.getStatus() == Error) {
		return -1;
	}

	MainController controller;
	controller.initialize();

	InputManager inputManager;
	EngineState state(&inputManager);

	while (!shouldClose(glfw)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		state.delta = glfwGetTime() - state.runtime;
		state.runtime += state.delta;

		controller.update(state);
		controller.render();

		glfwSwapBuffers(glfw.getWindow());

		inputManager.reset();
		glfwPollEvents();
		for (size_t i = 0; i < glfw.getNumInputEvents(); i++) {
			inputManager.registerEvent(glfw.getInputEvents()[i]);
			glfw.resetInputEvents();
		}

		state.tick++;
	}

	// set exit handler so that the stack deallocates before OpenGL releases the context
	std::atexit(terminate);
	return 0;
}
