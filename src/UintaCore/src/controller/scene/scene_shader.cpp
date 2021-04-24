#include <uinta/controller/scene/scene_shader.h>

using namespace uinta;

static const char *vertexShader =
		"#version 300 es\n"
		"layout (location = 0) in lowp vec3 inPos;"
		"layout (location = 1) in lowp vec3 inNormal;"
		"layout (location = 2) in lowp vec3 inColor;"
		"out lowp vec3 passNormal;"
		"out lowp vec3 passColor;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main() {"
		"	gl_Position = projection * view * model * vec4(inPos, 1.0f);"
		"	passNormal = inNormal;"
		"	passColor = inColor;"
		"}\0";

static const char *fragShader =
		"#version 300 es\n"
		"out lowp vec4 outColor;"
		"in lowp vec3 passNormal;"
		"in lowp vec3 passColor;"
		"const lowp vec3 lightDir = normalize(vec3(-1.0f, 1.0f, 1.0f));"
		"const lowp float lightIntensity = 2.f;"
		"void main() {"
		"	lowp float brightness = max(dot(lightDir, passNormal), 0.4f) * lightIntensity;"
		"	outColor = vec4(passColor * brightness, 1.0f);"
		"}\0";

SceneShader::SceneShader() : Shader(vertexShader, fragShader, Raw) {
	use();
}
