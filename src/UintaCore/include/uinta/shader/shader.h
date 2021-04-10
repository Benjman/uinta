#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include "shader_dto.h"
#include <uinta/gl_types.h>

namespace uinta {

	class ShaderDto;

	class Shader {
		friend class ShaderLoader;

	public:
		Shader(const char *vertex, const char *fragment, ShaderSourceType type);

		[[nodiscard]] GLuint getProgramId() const { return _id; }

		void use() const;

		void initialize() {
			use();
			loadUniforms();
		}

	protected:
		GLuint _id = GL_ZERO;

		virtual void loadUniforms() {}

	};

}

#endif //UINTA_SHADER_H
