#include <uinta/gl/gl_state.h>

#include <map>
#include <vector>

// TODO all these methods desperately need tests....

namespace uinta::gl_state {

#define VIEWPORT_SIZE_WIDTH 0
#define VIEWPORT_SIZE_HEIGHT 1

	const size_t GL_STATE_INVALID_ID = INT64_MAX;
	const uint8_t MAX_GL_STATE_TYPES = 100;

	// TODO should this be exposed in the header so we can create tests for it?
	void sanitizeKeyValuePair(size_t &key, size_t &value);

	namespace internal {
		extern std::map<size_t, size_t> glStates[MAX_GL_STATE_TYPES];
		std::map<size_t, size_t> glStates[MAX_GL_STATE_TYPES];
	}

	using namespace uinta::gl_state::internal;

	bool isActive(GlStateType type, size_t key, size_t secondary) {
		sanitizeKeyValuePair(key, secondary);
		containsState(type, true, key);
		return glStates[type][key] == secondary;
	}

	bool isActiveElseSet(GlStateType type, size_t key, size_t secondary) {
		bool vIsActive = isActive(type, key, secondary);
		if (!vIsActive) {
			setState(type, key, secondary);
		}
		return vIsActive;
	}

	bool containsState(GlStateType type, bool insert, size_t key) {
		bool contains = glStates->find(type) == glStates->end();
		if (!contains && insert) {
			glStates->insert(std::pair<size_t, size_t>(type, key));
		}
		return contains;
	}

	size_t getState(GlStateType type, size_t key) {
		return glStates[type][key];
	}

	void setState(GlStateType type, size_t key, size_t secondary) {
		sanitizeKeyValuePair(key, secondary);
		glStates[type][key] = secondary;
	}

	void setViewportSize(size_t width, size_t height) {
		setState(VIEWPORT_SIZE, VIEWPORT_SIZE_WIDTH, width);
		setState(VIEWPORT_SIZE, VIEWPORT_SIZE_HEIGHT, height);
	}

	void getViewportSize(size_t *widthPtr, size_t *heightPtr) {
		*widthPtr = getState(VIEWPORT_SIZE, VIEWPORT_SIZE_WIDTH);
		*heightPtr = getState(VIEWPORT_SIZE, VIEWPORT_SIZE_HEIGHT);
	}

	float_t getViewportAspectRatio() {
		size_t viewportW, viewportH;
		gl_state::getViewportSize(&viewportW, &viewportH);
		return (float_t) viewportW / (float_t) viewportH;
	}

	void sanitizeKeyValuePair(size_t &key, size_t &value) {
		/* If a state only has one value (such as GL_ARRAY_BUFFERS is set to _n), set key to zero.
		   Else, if a state is a thruple relationship (such as the currently bound vbo) we need to
		   track the state for all available options.

		   For example, the currently bound vbo (BOUND_BUFFER) has vbo id's for each separate _target;
		  		the BOUND_BUFFER for target GL_ARRAY_BUFFER might be _n, and;
		  		the BOUND_BUFFER for target GL_ELEMENT_ARRAY_BUFFER might be _n+1.

		   This allows for either option to be passed to us: */
		if (value == GL_STATE_INVALID_ID) {
			value = key;
			key = 0;
		}
	}

}

