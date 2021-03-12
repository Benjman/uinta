#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include <uinta/gl/gl_types.h>

namespace uinta {

	class File;
	class ShaderDto;

	class Shader {
		friend class ShaderLoader;

	public:
		static Shader createShader(ShaderDto &dto);

		[[nodiscard]] program_id_t getProgramId() const { return _id; }

		void use() const;

		~Shader();

	private:
		explicit Shader(program_id_t programId) : _id(programId) {}

		program_id_t _id;

	};

}

#endif //UINTA_SHADER_H
