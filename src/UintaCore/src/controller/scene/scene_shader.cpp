#include <uinta/controller/scene/scene_shader.h>

using namespace uinta;

static const char *vertexShader =
		"#version 300 es\n"
		"layout (location = 0) in lowp vec3 inPos;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main() {"
		"	gl_Position = projection * view * model * vec4(inPos, 1.0);"
		"}\0";

static const char *fragShader =
		"#version 300 es\n"
		"out lowp vec4 outColor;"
		"void main() {"
		"	outColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
		"}\0";

SceneShader::SceneShader() : Shader(vertexShader, fragShader, Raw) {
	use();
}
