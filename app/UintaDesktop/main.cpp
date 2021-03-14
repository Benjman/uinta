#include "glfw.h"

#include <uinta/shader.h>
#include <uinta/gl.h>
#include <uinta/font.h>

using namespace uinta;
using namespace uinta::glfw;

void exitHandler();

void debug();

int main() {
	GlfwDto dto(800, 600, "Test Window Creation");
//    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	ShaderDto shaderDto("/home/ben/Documents/shader.vert", "/home/ben/Documents/shader.frag");
	Shader shader = Shader::createShader(shaderDto);
	Font font = Font::loadFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

	float vertices[] = {
			// positions         // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f,
			0.5f, -0.5f, 0.0f,   1.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,   0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,   0.0f, 0.0f
	};
	unsigned int indices[] = {
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
	};

	Vao vao = Vao::requestVao();
	Vbo vertexBuffer = Vbo::requestVbo(&vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
	Vbo indexBuffer = Vbo::requestVbo(&vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices);
	vao.createAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
	vao.createAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		vao.bind();

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

void debug() {
}
