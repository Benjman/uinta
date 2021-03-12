#ifndef UINTA_SHADER_DTO_H
#define UINTA_SHADER_DTO_H

#include <uinta/types.h>

#include <cstring>

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

		ShaderDto(const char *vert, const char *frag, ShaderSourceType sourceType = IO)
				: vertPath(sourceType == IO ? vert : ""),
				  fragPath(sourceType == IO ? frag : ""),
				  vertSource(sourceType == Raw ? (char *) vert : (char *) ""),
				  fragSource(sourceType == Raw ? (char *) frag : (char *) ""),
				  sourceType(sourceType) {}

		~ShaderDto() {
			if (strlen(vertPath)) delete[] vertSource;
			if (strlen(fragPath)) delete[] fragSource;
		}
	};

}

#endif //UINTASHADERDTOH
