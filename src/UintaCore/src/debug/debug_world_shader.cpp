#include <uinta/debug/debug_world_shader.h>

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
  		"	outColor = vec4(1);"
		"}\0";

DebugWorldShader::DebugWorldShader() : Shader(vertexShader, fragShader, Raw) {

}
