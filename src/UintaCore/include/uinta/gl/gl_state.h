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
		VIEWPORT_SIZE,
	};

	extern bool isActive(GlStateType type, size_t key, size_t secondary = 0);

	extern bool isActiveElseSet(GlStateType type, size_t key, size_t secondary = INT32_MAX);

	extern bool containsState(GlStateType type, bool insert = false, size_t key = 0);

	extern size_t getState(GlStateType type, size_t key = 0);

	extern void setState(GlStateType type, size_t key, size_t secondary = INT32_MAX);

	extern void setViewportSize(size_t width, size_t height);

	extern void getViewportSize(size_t *width, size_t *height);

}

#endif //UINTA_GL_STATE_H
