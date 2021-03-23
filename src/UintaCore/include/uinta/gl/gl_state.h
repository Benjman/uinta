#ifndef UINTA_GL_STATE_H
#define UINTA_GL_STATE_H

#include <uinta/types.h>

namespace uinta::gl_state {

	extern const size_t GL_STATE_INVALID_ID;

	enum GlStateType {
		BOUND_BUFFER,
		BOUND_TEXTURE_2D,
		BOUND_VERTEX_ARRAY,
		SHADER_IN_USE,
		VIEWPORT_SIZE,
	};

	extern const uint8_t MAX_GL_STATE_TYPES;

	extern bool isActive(GlStateType type, size_t key, size_t secondary = GL_STATE_INVALID_ID);

	extern bool isActiveElseSet(GlStateType type, size_t key, size_t secondary = GL_STATE_INVALID_ID);

	extern bool containsState(GlStateType type, bool insert = false, size_t key = GL_STATE_INVALID_ID);

	extern size_t getState(GlStateType type, size_t key = 0);

	extern void setState(GlStateType type, size_t key, size_t secondary = GL_STATE_INVALID_ID);

	extern void setViewportSize(uint64_t width, uint64_t height);

	extern void getViewportSize(uint64_t *width, uint64_t *height);

}

#endif //UINTA_GL_STATE_H
