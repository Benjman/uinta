#ifndef UINTA_SMOOTH_VALUES_H
#define UINTA_SMOOTH_VALUES_H

#include <uinta/types.h>

#include <glm/vec3.hpp>

namespace uinta {

	// TODO this should be a component
	struct SmoothFloat {
		float_t target;
		float_t current;
		float_t agility;

		SmoothFloat(float_t value, float_t agility) : target(value), current(value), agility(agility) {}

		void update(float_t dt) {
			float_t diff = target - current;
			current += diff * dt * agility;
		}

		SmoothFloat operator+=(float_t v) {
			target += v;
			return *this;
		}

		SmoothFloat operator-=(float_t v) {
			target -= v;
			return *this;
		}

		SmoothFloat &operator=(float_t v) {
			target = v;
			return *this;
		}

		operator float_t() const {
			return current;
		}

	}; // struct SmoothFloat

	struct SmoothVector3 {
		glm::vec3 current = glm::vec3(0);
		glm::vec3 target = glm::vec3(0);

		float_t agility;

		SmoothVector3(glm::vec3 target, float_t agility) : current(target), target(target), agility(agility) {}

		void update(const float_t dt) {
			float_t factor = dt * agility;
			if (factor > 1.f) {
				current = target;
			} else {
				glm::vec3 diff = target - current;
				diff *= factor;
				current += diff;
			}
		}

		operator glm::vec3() const {
			return current;
		}

	}; // struct SmoothVector3

} // namespace uinta

#endif //UINTASMOOTHVALUESH
