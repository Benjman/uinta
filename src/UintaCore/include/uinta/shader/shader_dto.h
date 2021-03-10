#ifndef UINTA_SHADER_DTO_H
#define UINTA_SHADER_DTO_H

#include <uinta/types.h>

namespace uinta {

	enum ShaderSourceType {
		IO,
		Raw,
	};

	struct ShaderDto {
		friend class ShaderLoader;

		ShaderSourceType sourceType;

		const char *vertPath;
		const char *fragPath;

		char *vertSource{};
		char *fragSource{};

		size_t vertLength;
		size_t fragLength;

		ShaderDto(const char *vertPath, const char *fragPath)
				: ShaderDto(IO, vertPath, fragPath) {}

		ShaderDto(ShaderSourceType sourceType, const char *vert, const char *frag)
				: sourceType(sourceType),
				  vertPath(sourceType == IO ? vert : ""),
				  fragPath(sourceType == IO ? frag : "") {}

		~ShaderDto() {
			delete[] vertSource;
			delete[] fragSource;
		}
	};

}

#endif //UINTASHADERDTOH
