#ifndef UINTA_DEBUGUIVIEW_H
#define UINTA_DEBUGUIVIEW_H

#include <uinta/types.h>
#include <uinta/text/text.h>

namespace uinta {

	class DebugController;

	struct DebugUiView {
		const Text fpsLbl = initializeComponent("FPS:", 10, 0);
		Text fps = initializeComponent("0", 70, 0);
		const Text tickLabel = initializeComponent("Tick:", 10, 25);
		Text tick = initializeComponent("0", 70, 25);

	private:
		static Text initializeComponent(const char *v, size_t x, size_t y) {
			Text t(v);
			t.setPositionPx(x, y);
			return t;
		}

	}; // struct DebugUiView

} // namespace uinta

#endif // UINTA_DEBUGUIVIEW_H


