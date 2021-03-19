#include <uinta/gl/gl_state.h>

#include <map>
#include <vector>

// TODO all these methods desperately need tests....

namespace uinta::gl_state {

#define VIEWPORT_SIZE_WIDTH 0
#define VIEWPORT_SIZE_HEIGHT 1

	void safeKvp(size_t &key, size_t &secondary);

	namespace internal {
		extern std::map<size_t, size_t> glStates[MAX_GL_STATE_TYPES];
		std::map<size_t, size_t> glStates[MAX_GL_STATE_TYPES];
	}

	using namespace uinta::gl_state::internal;

	bool isActive(GlStateType type, size_t key, size_t secondary) {
		safeKvp(key, secondary);
		containsState(type, true, key);
		return glStates[type][key] == secondary;
	}

	bool isActiveElseSet(GlStateType type, size_t key, size_t secondary) {
		safeKvp(key, secondary);
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
		safeKvp(key, secondary);
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

	void safeKvp(size_t &key, size_t &secondary) {
		if (secondary == INT32_MAX) {
			secondary = key;
			key = 0;
		}
	}

}

