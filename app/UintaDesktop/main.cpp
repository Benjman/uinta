#include <uinta/engine_state.h>
#include "glfw.h"
#include "proto/protos.h"

#include <uinta/io.h>
#include <uinta/shader.h>
#include <uinta/text.h>

using namespace uinta;
using namespace uinta::glfw;

void exitHandler();

int main() {
	GlfwDto dto(1282, 532, "Test Window Creation");
//	dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	EngineState state;

	DebugUiProto proto;
	proto.initialize();

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		state.delta = glfwGetTime() - state.runtime;
		state.runtime += state.delta;

		proto.update(state);
		proto.render();

		glfwSwapBuffers(dto.getWindow());
		glfwPollEvents();

		state.tick++;
	}

	std::atexit(exitHandler);
	return 0;
}

void exitHandler() {
	terminate();
}
