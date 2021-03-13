#include "glfw.h"

#include <uinta/shader.h>
#include <uinta/gl.h>

using namespace uinta;
using namespace uinta::glfw;

void exitHandler();

int main() {
	std::atexit(exitHandler);

	GlfwDto dto(800, 600, "Test Window Creation");
//    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	ShaderDto shaderDto("/home/ben/Documents/shader.vert", "/home/ben/Documents/shader.frag");
	Shader shader = Shader::createShader(shaderDto);

	float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left
			0.5f, -0.5f, 0.0f, // right
			0.0f, 0.5f, 0.0f  // top
	};

	Vao vao = Vao::requestVao();
	Vbo vbo = Vbo::requestVbo(&vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
	vao.createAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glCheckError(GL_DRAW_ARRAYS);

		glfwSwapBuffers(dto.getWindow());

		glfwPollEvents();
	}

	return 0;
}


void exitHandler() {
	terminate();
}
