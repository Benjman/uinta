#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include "shader_types.h"

namespace uinta {

	class File;
	class ShaderDto;

	class Shader {
		friend class ShaderLoader;

	public:
		static Shader createShader(ShaderDto &dto);

		[[nodiscard]] program_id_t getProgramId() const { return _programId; }

	private:
		explicit Shader(program_id_t programId) : _programId(programId) {}

		program_id_t _programId;

	};

}

#endif //UINTA_SHADER_H
