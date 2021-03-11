#include "glfw.h"

#include <uinta/shader.h>

using namespace uinta;
using namespace uinta::glfw;

void debug();

int main() {
	GlfwDto dto(800, 600, "Test Window Creation");
    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	debug();

//	while (!shouldClose(dto)) {
		glfwSwapBuffers(dto.getWindow());
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
//	}

	terminate();
	return 0;
}

void debug() {
	VertexAttribute attribute(0, 3, GL_FLOAT, false, 3 * sizeof(float_t), 0);
}
