#include "glfw.h"

#include <uinta/shader.h>
#include <uinta/gl.h>

using namespace uinta;
using namespace uinta::glfw;

void debug(GlfwDto &dto);

int main() {
	GlfwDto dto(800, 600, "Test Window Creation");
//    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	debug(dto);

//	while (!shouldClose(dto)) {
	glfwSwapBuffers(dto.getWindow());
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
//	}

	terminate();
	return 0;
}

void debug(GlfwDto &dto) {
	const char *vertexShaderSource = "#version 330 core\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "void main()\n"
									 "{\n"
									 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
									 "}\0";
	const char *fragmentShaderSource = "#version 330 core\n"
									   "out vec4 FragColor;\n"
									   "void main()\n"
									   "{\n"
									   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									   "}\n\0";

	ShaderDto shaderDto(vertexShaderSource, fragmentShaderSource, Raw);
	Shader shader = Shader::createShader(shaderDto);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left
			0.5f, -0.5f, 0.0f, // right
			0.0f, 0.5f, 0.0f  // top
	};

	Vao vao = Vao::requestVao();
	Vbo vbo = Vbo::requestVbo(&vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
	VertexAttribute attribute(&vao, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glCheckError(GL_CLEAR);

		shader.use();
		vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glCheckError(GL_DRAW_ARRAYS);

		glfwSwapBuffers(dto.getWindow());
		glfwPollEvents();
	}
//	other(dto);
}
