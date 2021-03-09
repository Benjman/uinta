#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include "shader_dto.h"

#include <GLES3/gl3.h>

namespace uinta {

	class File;
	class ShaderDto;

	class Shader {
		friend class ShaderLoader;

	public:
		static Shader createShader(ShaderDto &dto);

		[[nodiscard]] GLuint getGlId() const { return _glId; }

	private:
		explicit Shader(GLuint glId) : _glId(glId) {}

		GLuint _glId;

	};

}

#endif //UINTA_SHADER_H
