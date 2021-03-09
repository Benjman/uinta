#include "glfw.h"

#include <uinta/exception.h>
#include <uinta/shader.h>

using namespace uinta;
using namespace uinta::glfw;

#include <iostream>
#include <string>

void debug();

int main(const int argc, const char **argv) {
	GlfwDto dto(800, 600, "Test Window Creation");
//    dto.setHeadless(true);

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
	try {
		ShaderDto dto("/home/ben/Documents/shader.vert", "/home/ben/Documents/shader.frag");
		Shader shader = Shader::createShader(dto);
	} catch (const Exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
