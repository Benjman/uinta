#include "glfw.h"

#include <uinta/debug.h>
#include <uinta/gl.h>
#include <uinta/io.h>
#include <uinta/shader.h>
#include <uinta/text.h>

using namespace uinta;
using namespace uinta::gl_state;
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
//	dto.setHeadless(true);

	if (!initialize(dto) || dto.getStatus() == Error) {
		return -1;
	}

	DebugController debugController;

//	ShaderDto shaderDto(vertShader, fragShader, Raw);
	ShaderDto shaderDto("/home/ben/Documents/shader.vert", "/home/ben/Documents/shader.frag");
	Shader shader = Shader::createShader(shaderDto);
	Font font = Font::loadFont("/usr/share/fonts/TTF/AkaashNormal.ttf");

	Text text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque quis dui ac dolor ultrices tempor et eu elit. Phasellus tristique quam tellus, eu pharetra neque venenatis eu. Pellentesque faucibus volutpat maximus. Vestibulum lobortis erat id tellus dignissim, a viverra magna consectetur. Maecenas imperdiet purus sapien, eget cursus elit imperdiet vel. Nullam urna tortor, interdum sed eros eu, vestibulum molestie quam. Fusce ut sapien facilisis, facilisis libero eget, semper nibh. Nunc mattis mi eget maximus maximus. Proin venenatis tortor in vulputate iaculis. Vivamus ut erat quis lorem porta pharetra. Vivamus eget convallis augue, in lacinia sem. Sed sit amet convallis nulla. Cras sed odio non orci luctus ornare.  Sed aliquet ligula vel faucibus consectetur. Pellentesque nec quam tortor. Vivamus eu justo a nulla faucibus congue. Aenean vulputate rhoncus sagittis. Nulla id eleifend dolor. Vivamus luctus tortor sagittis eros tempor auctor. Suspendisse semper nisl ut nulla mattis tristique. Sed at tellus pellentesque velit venenatis mattis.  Duis vestibulum porta dolor, in mattis mi euismod a. Curabitur at erat nulla. Maecenas bibendum est auctor tortor luctus convallis. Maecenas eu neque at urna bibendum efficitur ut eu velit. Vivamus dolor eros, malesuada in libero nec, convallis pulvinar libero. Quisque sed facilisis augue, ut rutrum felis. Maecenas risus nisl, dignissim sit amet elit eget, tempus pretium justo.  Vestibulum semper nisi sed libero fringilla, gravida feugiat sapien venenatis. Nullam tincidunt leo ligula, et eleifend felis elementum eu. Proin augue lorem, suscipit sit amet finibus at, hendrerit quis urna. Aliquam erat volutpat. Donec quis nunc consequat, dictum eros ut, placerat felis. Donec pellentesque turpis id aliquam aliquet. Integer eu tellus vitae justo sagittis porta ut nec neque.  Nam mattis semper est quis sodales. Aliquam sem mauris, volutpat ac pharetra eget, porttitor quis orci. Aenean sed posuere sem. Curabitur a orci nec nisi rhoncus hendrerit et in elit. Nulla facilisi. Suspendisse at nibh et nulla vehicula dignissim. Nullam finibus sem sapien, nec tempus erat ultrices nec. Phasellus non risus nulla. Cras dignissim auctor neque, id sollicitudin mi pulvinar porta. Nullam condimentum leo eget turpis sollicitudin scelerisque. Aliquam hendrerit velit vel massa accumsan, at congue justo eleifend. Sed faucibus fermentum eros. Sed diam nisi, commodo a fringilla sit amet, consectetur vitae dui. Donec id ultrices augue, in scelerisque ipsum. Nullam dictum tincidunt viverra. ", &font);
	size_t charCount = text.getNonSpaceCharacterCount();
	float_t interleavedData[charCount * Text::VERTICES_PER_CHAR * 2];
	uint32_t indices[charCount * Text::INDICES_PER_CHAR];
	text.generateMesh(interleavedData, indices);

	Vao vao;
	Vbo vertexBuffer = Vbo::requestVbo(&vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(interleavedData), interleavedData);
	Vbo indexBuffer = Vbo::requestVbo(&vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices);
	vao.createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float_t), (void *) (0 * sizeof(float_t)));
	vao.createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float_t), (void *) (2 * sizeof(float_t)));

	if (!isActiveElseSet(ENABLE, GL_BLEND, GL_TRUE)) {
		glEnable(GL_BLEND);
		glCheckError(GL_ENABLE);
	}

	if (!isActiveElseSet(BLEND_FUNC, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCheckError(GL_BLEND_FUNC);
	}

	while (!shouldClose(dto)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (dto.isViewportChanged()) {
			/* debugController.getUi().updateBuffer(); */
			/* dto.resetViewportChanged(); */
		}

		shader.use();
//		debugController.render();

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
