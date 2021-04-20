#include "glfw.h"

#include <uinta/camera.h>
#include <uinta/controller/scene/scene_controller.h>
#include <uinta/debug/debug_controller.h>
#include <uinta/engine_state.h>
#include <uinta/input/input_manager_impl.h>

using namespace uinta;
using namespace uinta::glfw;

class MainController : public Controller, public IRenderController {
	PerspectiveCamera camera;

	DebugController debug = DebugController(this, &camera);
	SceneController scene = SceneController(this, &camera);

public:
	MainController() : Controller(nullptr) {
		camera.initialize();
	}

	void update(const EngineState &state) override {
		camera.update(state);

		Controller::update(state);
	}

	void render() override {
		scene.render();
		debug.render();
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

	InputManagerImpl inputManager;
	EngineState state(&inputManager);

	while (!shouldClose(glfw)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		state.delta = (float_t) glfwGetTime() - state.runtime;
		state.runtime += state.delta;

		controller.update(state);
		controller.render();

		glfwSwapBuffers(glfw.getWindow());

		glfwPollEvents();
		glfw.updateInputState(&inputManager);

		state.tick++;
	}

	// set exit handler so that the stack deallocates before OpenGL releases the context
	std::atexit(terminate);
	return 0;
}
