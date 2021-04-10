#ifndef UINTA_MATH_H
#define UINTA_MATH_H

#include <uinta/types.h>

namespace uinta {

	static constexpr float_t PI = 3.1415926535f;

	inline float_t rad(float_t deg) {
		return deg * (PI / 180.f);
	}

}

#endif //UINTA_MATH_H
