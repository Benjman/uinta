#include "glfw.h"

#include <uinta/debug.h>
#include <uinta/gl.h>
#include <uinta/shader.h>
#include <uinta/text.h>
#include <uinta/ui.h>

using namespace uinta;
using namespace uinta::glfw;

void exitHandler();

static const char *fragShader = "#version 300 es\n" \
								 "out lowp vec4 outColor;" \
								 "void main() {" \
								 "	outColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);" \
								 "}";

static const char *vertShader = "#version 300 es\n" \
								 "layout (location = 0) in lowp vec2 inPos;" \
								 "void main() {" \
								 "	gl_Position = vec4(inPos, 0.0, 1.0);" \
								 "}";

int main() {
	GlfwDto dto(1000, 1000, "Test Window Creation");
//    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	DebugController debugController;

	ShaderDto shaderDto(vertShader,fragShader, Raw);
	Shader shader = Shader::createShader(shaderDto);

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (dto.isViewportChanged()) {
			debugController.getUi().updateBuffer();
			dto.resetViewportChanged();
		}

		shader.use();
		debugController.render();

		glfwSwapBuffers(dto.getWindow());

		glfwPollEvents();
	}

	std::atexit(exitHandler);
	return 0;
}

void exitHandler() {
	terminate();
}

