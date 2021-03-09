#include "shader_loader.h"

#include <uinta/shader/shader.h>
#include <uinta/shader/shader_dto.h>

uinta::Shader uinta::Shader::createShader(ShaderDto &dto) {
	return ShaderLoader::loadShader(dto);
}
