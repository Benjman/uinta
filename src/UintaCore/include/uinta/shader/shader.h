#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include "shader_dto.h"
#include <uinta/gl/gl_types.h>

namespace uinta {

	class ShaderDto;

	class Shader {
		friend class ShaderLoader;

	public:
		static Shader *createShader(const char *vertex, const char *fragment, ShaderSourceType type);

		[[nodiscard]] program_id_t getProgramId() const { return _id; }

		void use() const;

	private:
		explicit Shader(program_id_t programId) : _id(programId) {}

		const program_id_t _id;

	};

}

#endif //UINTA_SHADER_H
