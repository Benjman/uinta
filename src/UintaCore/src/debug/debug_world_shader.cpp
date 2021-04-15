#include <uinta/debug/debug_world_shader.h>

using namespace uinta;

static const char *vertexShader =
		"#version 300 es\n"
		"layout (location = 0) in lowp vec3 inPos;"
		"layout (location = 1) in lowp vec3 inColor;"
		"out lowp vec3 passColor;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main() {"
		"	gl_Position = projection * view * model * vec4(inPos, 1.0);"
		"	passColor = inColor;"
		"}\0";

static const char *fragShader =
		"#version 300 es\n"
		"in lowp vec3 passColor;"
		"out lowp vec4 outColor;"
		"void main() {"
		"	outColor = vec4(passColor, 1.0);"
		"}\0";

DebugWorldShader::DebugWorldShader() : Shader(vertexShader, fragShader, Raw) {
}

void DebugWorldShader::initializeAttributes(Vao *vao) {
	Shader::initializeAttributes(vao);
	vao->createAttribute(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao->createAttribute(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
}
