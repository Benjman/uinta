#include <uinta/controller/scene/scene_shader.h>

using namespace uinta;

static const char *vertexShader =
		"#version 300 es\n"
		"layout (location = 0) in lowp vec3 inPos;"
		"layout (location = 1) in lowp vec3 inNormal;"
		"out lowp vec3 passNormal;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main() {"
		"	gl_Position = projection * view * model * vec4(inPos, 1.0);"
		"	passNormal = inNormal;"
		"}\0";

static const char *fragShader =
		"#version 300 es\n"
		"out lowp vec4 outColor;"
		"in lowp vec3 passNormal;"
		"const lowp vec3 lightDir = normalize(vec3(0.4f, 1.0f, 0.7f));"
		"void main() {"
		"	lowp float brightness = max(dot(lightDir, passNormal), 0.3);"
		"	outColor = vec4(vec3(1.0f, 0.5f, 0.2f) * brightness, 1.0f);"
		"}\0";

SceneShader::SceneShader() : Shader(vertexShader, fragShader, Raw) {
	use();
}
