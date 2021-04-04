#include "uinta/debug/debug_ui_view.h"

#include <uinta/text.h>

namespace uinta {

	namespace debuguiview {

		Text initialize(const char *v, size_t x, size_t y) {
			Text t(v);
			t.setPositionPx(x, y);
			return t;
		}

	}
	using namespace debuguiview;

	DebugUiView::DebugUiView() :
			fps(initialize("0", 90, 0)), // keep spaces to allocate enough space on gpu for 3 chars long FPS value
			fpsLbl(initialize("FPS:", 15, 0)) {
	}

}
