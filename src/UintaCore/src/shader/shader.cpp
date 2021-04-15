#include "shader_loader.h"

#include <uinta/shader.h>

#include <uinta/gl/gl_state.h>

using namespace uinta;
using namespace uinta::gl_state;

Shader::Shader(const char *vertex, const char *fragment, ShaderSourceType type) {
	ShaderDto dto(vertex, fragment, type);
	_id = ShaderLoader::loadShader(dto);
}

void Shader::use() const {
	if (!isActiveElseSet(SHADER_IN_USE, _id)) {
		glUseProgram(_id);
		glCheckError(GL_USE_PROGRAM);
	}
}

void Shader::initializeAttributes(Vao *vao) {
	vao->bind();
}
