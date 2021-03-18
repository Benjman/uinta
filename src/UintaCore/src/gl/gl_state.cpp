#include <uinta/gl/gl_state.h>

#include <map>
#include <vector>

// TODO all these methods desperately need tests....

namespace uinta::gl_state {

	namespace internal {
		extern std::map<size_t, size_t> glStates[MAX_GL_STATE_TYPES];
		std::map<size_t, size_t> glStates[MAX_GL_STATE_TYPES];

		extern float_t _aspectRatio;
		float_t _aspectRatio = 1;
	}

	using namespace uinta::gl_state::internal;

	bool isActive(GlStateType type, size_t key, size_t secondary) {
		if (secondary == INT32_MAX) {
			// Method called without secondary value:
			secondary = key;
			key = 0;
		}
		auto iterator = glStates->find(type);
		if (iterator == glStates->end()) {
			glStates->insert(std::pair<size_t, size_t>(type, key));
		}
		return glStates[type][key] == secondary;
	}

	bool isActiveElseSet(GlStateType type, size_t key, size_t secondary) {
		if (secondary == INT32_MAX) {
			// Method called without secondary value:
			secondary = key;
			key = 0;
		}
		bool vIsActive = isActive(type, key, secondary);
		if (!vIsActive) {
			glStates[type][key] = secondary;
		}
		return vIsActive;
	}

	void setCurrentAspectRatio(float_t aspectRatio) {
		_aspectRatio = aspectRatio;
	}

	float_t getCurrentAspectRatio() {
		return _aspectRatio;
	}

}

