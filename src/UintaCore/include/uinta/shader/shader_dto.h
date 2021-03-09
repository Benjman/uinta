#ifndef UINTA_SHADER_DTO_H
#define UINTA_SHADER_DTO_H

#include <uinta/io.h>

namespace uinta {

	enum ShaderSourceType {
		IO,
		Raw,
	};

	class ShaderDto {
		friend class ShaderLoader;

		ShaderSourceType _sourceType;

		const char *_vertPath;
		const char *_fragPath;

		char *_vertSource{};
		char *_fragSource{};

		size_t _vertLength;
		size_t _fragLength;

	public:
		ShaderDto(const char *vertPath, const char *fragPath)
				: ShaderDto(IO, vertPath, fragPath) {}

		ShaderDto(ShaderSourceType sourceType, const char *vert, const char *frag)
				: _sourceType(sourceType),
				  _vertPath(sourceType == IO ? vert : ""),
				  _fragPath(sourceType == IO ? frag : "") {}

		~ShaderDto() {
			delete[] _vertSource;
			delete[] _fragSource;
		}
	};

}

#endif //UINTA_SHADER_DTO_H
