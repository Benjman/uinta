#ifndef UINTA_GL_STATE_H
#define UINTA_GL_STATE_H

#include <uinta/types.h>

namespace uinta::gl_state {

	enum GlStateType {
		BOUND_BUFFER,
		BOUND_TEXTURE_2D,
		BOUND_VERTEX_ARRAY,
		SHADER_IN_USE,
		VIEWPORT_SIZE,
	};

	extern const uint8_t MAX_GL_STATE_TYPES;

	extern bool isActive(GlStateType type, size_t key, size_t secondary = INT32_MAX);

	extern bool isActiveElseSet(GlStateType type, size_t key, size_t secondary = INT32_MAX);

	extern bool containsState(GlStateType type, bool insert = false, size_t key = INT32_MAX);

	extern size_t getState(GlStateType type, size_t key = 0);

	extern void setState(GlStateType type, size_t key, size_t secondary = INT32_MAX);

	extern void setViewportSize(size_t width, size_t height);

	extern void getViewportSize(size_t *width, size_t *height);

}

#endif //UINTA_GL_STATE_H
