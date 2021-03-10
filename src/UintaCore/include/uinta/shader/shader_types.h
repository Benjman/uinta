#ifndef UINTA_SHADER_TYPES_H
#define UINTA_SHADER_TYPES_H

#include <GLES3/gl3.h>

namespace uinta {
	using program_id_t = GLuint;
	using location_id_t = GLint;

	static inline const location_id_t INVALID_LOCATION_ID = -1;
}

#endif //UINTA_SHADER_TYPES_H
