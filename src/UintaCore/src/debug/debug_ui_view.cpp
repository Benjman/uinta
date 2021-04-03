#include "uinta/debug/debug_ui_view.h"
#include "uinta/debug/debug_ui_controller.h"

#include <uinta/text.h>

namespace uinta {

	namespace debuguiview {

		Text initialize(const char *v, size_t x) {
			Text t(v);
			t.setPositionPx(x, 0);
			return t;
		}

	}
	using namespace debuguiview;

	DebugUiView::DebugUiView() :
			fps(initialize("xxx", 90)), // keep spaces to allocate enough space on gpu for 3 chars long FPS value
			fpsLbl(initialize("FPS:", 15)) {
	}

}
