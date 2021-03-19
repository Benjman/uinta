#include "uinta/gl/gl_state.h"
#include <cstring>
#include <uinta/types.h>
#include <uinta/ui/ui.h>
#include <uinta/ui/ui_element.h>

#include <iostream>

namespace uinta {

	void UiElement::generateMesh(float_t *data, uint32_t *indices) {
		size_t viewportWidth = 0,
				viewportHeight = 0;
		gl_state::getViewportSize(&viewportWidth, &viewportHeight);

		float_t x = _position.x;
		float_t w = x + _size.x;

		float_t y = _position.y;
		float_t h = y + _size.y;

		x /= viewportWidth;
		w /= viewportWidth;
		y /= viewportHeight;
		h /= viewportHeight;

		// convert them from unit space ( [0, 1] ) to gl ndc ( [-1, 1] )
		// f(x) = 2x-1
		float_t gl_x = 2 * x - 1;
		float_t gl_w = 2 * w - 1;
		// f(y) = -2x+1
		float_t gl_y = -2 * y + 1;
		float_t gl_h = -2 * h + 1;

		/*
		 v0  _____  v3
			|     |
			|     |
		 v1  -----  v2
		 */
		const float_t buf[8]{
				gl_x, gl_y,
				gl_x, gl_h,
				gl_w, gl_h,
				gl_w, gl_y
		};
		const uint32_t idx[6]{
				0, 1, 3,
				1, 2, 3
		};

		std::memcpy(data, buf, sizeof(buf));
		std::memcpy(indices, idx, sizeof(idx));
	}

}
