#include <uinta/debug/debug_ui_shader.h>
#include <uinta/shader/vao.h>

using namespace uinta;

static const char *vertexShader =
		"#version 300 es\n"
		"layout (location = 0) in lowp vec2 inPos;"
		"layout (location = 1) in lowp vec2 inUv;"
		"out lowp vec2 passUv;"
		"void main() {"
		"	gl_Position = vec4(inPos, 0.0, 1.0);"
		"	passUv = vec2(inUv.x, inUv.y);"
		"}\0";

static const char *fragShader =
		"#version 300 es\n"
		"in lowp vec2 passUv;"
		"out lowp vec4 outColor;"
		"uniform sampler2D texture1;"
		"void main() {"
		"	outColor = vec4(texture(texture1, passUv).r);"
		"}\0";

DebugUiShader::DebugUiShader() : Shader(vertexShader, fragShader, Raw) {
}

void DebugUiShader::initialize(Vao &vao) {
	vao.bind();
	vao.createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao.createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
}
