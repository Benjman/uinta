#include "glfw.h"
#include "proto/protos.h"

#include <uinta/io.h>
#include <uinta/shader.h>
#include <uinta/text.h>

using namespace uinta;
using namespace uinta::glfw;

void exitHandler();

int main() {
	GlfwDto dto(1920, 1080, "Test Window Creation");
//	dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	DebugUiProto proto;

	float_t runtime = 0, delta;
	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		delta = glfwGetTime() - runtime;
		runtime += delta;

		if (dto.isViewportChanged()) {
			proto.viewportChanged(dto.getWidth(), dto.getHeight());
		}

		proto.update(delta);
		proto.render();

		glfwSwapBuffers(dto.getWindow());
		glfwPollEvents();
	}

	std::atexit(exitHandler);
	return 0;
}

void exitHandler() {
	terminate();
}
