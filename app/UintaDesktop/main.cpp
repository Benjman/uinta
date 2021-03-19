#include "glfw.h"

#include <uinta/debug.h>
#include <uinta/gl.h>
#include <uinta/shader.h>
#include <uinta/text.h>
#include <uinta/ui.h>

using namespace uinta;
using namespace uinta::glfw;

void exitHandler();

int main() {
	GlfwDto dto(1000, 1000, "Test Window Creation");
//    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	DebugController debugController;

	ShaderDto shaderDto("/home/ben/Documents/shader.vert", "/home/ben/Documents/shader.frag");
	Shader shader = Shader::createShader(shaderDto);

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (dto.isViewportChanged()) {
			debugController.getUi().updateBuffer();
			dto.resetViewportChanged();
		}

		shader.use();
		debugController.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glCheckError(GL_DRAW_ELEMENTS);

		glfwSwapBuffers(dto.getWindow());

		glfwPollEvents();
	}

	std::atexit(exitHandler);
	return 0;
}

void exitHandler() {
	terminate();
}
