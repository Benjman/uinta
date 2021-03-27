#include "glfw.h"

#include <uinta/debug.h>
#include <uinta/gl.h>
#include <uinta/io.h>
#include <uinta/shader.h>
#include <uinta/text.h>

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
	GlfwDto dto(1920, 1080, "Test Window Creation");
//    dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	DebugController debugController;

//	ShaderDto shaderDto(vertShader, fragShader, Raw);
	ShaderDto shaderDto("/home/ben/Documents/shader.vert", "/home/ben/Documents/shader.frag");
	Shader shader = Shader::createShader(shaderDto);
	Font font = Font::loadFont("/usr/share/fonts/TTF/Hack-Bold.ttf");

	Text text("I am happy to finally see the beginnings of text rendering.", &font);
	size_t charCount = text.getNonSpaceCharacterCount();
	float_t interleavedData[charCount * Text::VERTICES_PER_CHAR * 2];
	uint32_t indices[charCount * Text::INDICES_PER_CHAR];
	text.generateMesh(interleavedData, indices);

	Vao vao;
	Vbo vertexBuffer = Vbo::requestVbo(&vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(interleavedData), interleavedData);
	Vbo indexBuffer = Vbo::requestVbo(&vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices);
	vao.createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float_t), (void *) (0 * sizeof(float_t)));
	vao.createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float_t), (void *) (2 * sizeof(float_t)));

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (dto.isViewportChanged()) {
			/* debugController.getUi().updateBuffer(); */
			/* dto.resetViewportChanged(); */
		}

		shader.use();
//		debugController.render();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawElements(GL_TRIANGLES, charCount * 6, GL_UNSIGNED_INT, 0);
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

