#include "glfw.h"
#include "proto/protos.h"

#include <uinta/engine_state.h>
#include <uinta/input/input_manager.h>
#include <uinta/shader.h>
#include <uinta/text.h>

using namespace uinta;
using namespace uinta::glfw;

int main() {
	GlfwDto glfw(1282, 532, "Test Window Creation");
//	glfw.setHeadless(true);

	if (!initialize(glfw) || glfw.getStatus() == Error) {
		return -1;
	}

	InputManager inputManager;
	EngineState state(&inputManager);

	DebugUiProto proto;
	proto.initialize();

	while (!shouldClose(glfw)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		state.delta = glfwGetTime() - state.runtime;
		state.runtime += state.delta;

		proto.update(state);
		proto.render();

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
