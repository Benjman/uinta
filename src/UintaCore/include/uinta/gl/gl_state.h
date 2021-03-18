#ifndef UINTA_GL_STATE_H
#define UINTA_GL_STATE_H

#include "gl_types.h"

#include <uinta/types.h>

namespace uinta::gl_state {

	static const uint8_t MAX_GL_STATE_TYPES = 100;

	enum GlStateType {
		BOUND_BUFFER,
		BOUND_TEXTURE_2D,
		BOUND_VERTEX_ARRAY,
		SHADER_IN_USE,
	};

	extern bool isActive(GlStateType type, size_t key, size_t secondary = 0);

	extern bool isActiveElseSet(GlStateType type, size_t key, size_t secondary = INT32_MAX);

	extern void setCurrentAspectRatio(float_t aspectRatio);

	extern float_t getCurrentAspectRatio();

}

#endif //UINTA_GL_STATE_H
