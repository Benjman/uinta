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
			0.5f,  0.5f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left
	};
	unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
	};

	Vao vao = Vao::requestVao();
	Vbo vertexBuffer = Vbo::requestVbo(&vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
	Vbo indexBuffer = Vbo::requestVbo(&vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices);
	vao.createAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		vao.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glCheckError(GL_DRAW_ELEMENTS);

		glfwSwapBuffers(dto.getWindow());

		glfwPollEvents();
	}

	return 0;
}


void exitHandler() {
	terminate();
}
