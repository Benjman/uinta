#include <uinta/gl.h>
#include <uinta/types.h>
#include <uinta/ui/ui.h>
#include <uinta/ui/ui_element.h>

namespace uinta {

	void UiElement::generateMesh(float_t *data, uint32_t *indices) {
		float_t aspectRatio = gl_state::getCurrentAspectRatio();
		size_t index = 0;

		float_t x0 = ((float_t) _position.x / UI_BASE_WIDTH) * (aspectRatio > 1 ? aspectRatio : 1);
		float_t y0 = ((float_t) _position.y / UI_BASE_HEIGHT);
		float_t x1 = (((float_t) _position.x + (float_t) _size.x) / UI_BASE_WIDTH) * (aspectRatio > 1 ? aspectRatio : 1);
		float_t y1 = ((float_t) _position.y + (float_t) _size.y) / UI_BASE_HEIGHT;

		data[index++] = x0;
		data[index++] = y0 * aspectRatio;
		data[index++] = x0;
		data[index++] = y1 * aspectRatio;
		data[index++] = x1;
		data[index++] = y1 * aspectRatio;
		data[index++] = x1;
		data[index  ] = y0 * aspectRatio;

		// convert x to ndc: [0, 1] -> [-1, 1]	f(x) = 2x-1
		for (size_t i = 0; i < 8; i += 2)
		{
			data[i] = 2 * data[i] - 1;
		}

		// convert y to ndc: [0, 1] -> [-1, 1]	f(y) = -(2y-1)
		for (size_t i = 1; i < 8; i += 2) {
			data[i] = -(2 * data[i] - 1);
		}

		/* set ebo
		 v0  _____  v3
			|     |
			|     |
		 v1  -----  v2
		 */
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 3;
		indices[3] = 1;
		indices[4] = 2;
		indices[5] = 3;

	}

}
