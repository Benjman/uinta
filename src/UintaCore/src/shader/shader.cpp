#include "shader_loader.h"

#include <uinta/shader/shader.h>
#include <uinta/shader/shader_dto.h>
#include <uinta/gl/gl_error.h>

uinta::Shader uinta::Shader::createShader(ShaderDto &dto) {
	return ShaderLoader::loadShader(dto);
}

void uinta::Shader::use() const {
	// TODO check GL state
	glUseProgram(_id);
	glCheckError(GL_USE_PROGRAM);
}
