#include "shader_loader.h"

#include <uinta/shader/shader.h>
#include <uinta/shader/shader_dto.h>

#include <uinta/gl/gl_error.h>
#include <uinta/gl/gl_state.h>

using namespace uinta;
using namespace uinta::gl_state;

Shader *Shader::createShader(const char *vertex, const char *fragment, ShaderSourceType type) {
	ShaderDto dto(vertex, fragment, type);
	return ShaderLoader::loadShader(dto);
}

void Shader::use() const {
	if (!isActiveElseSet(SHADER_IN_USE, _id)) {
		glUseProgram(_id);
		glCheckError(GL_USE_PROGRAM);
	}
}
