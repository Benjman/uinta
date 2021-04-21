#ifndef UINTA_SMOOTH_FLOAT_H
#define UINTA_SMOOTH_FLOAT_H

#include <uinta/types.h>

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

} // namespace uinta

#endif //UINTA_SMOOTH_FLOAT_H
