#ifndef UINTA_DEBUGUIVIEW_H
#define UINTA_DEBUGUIVIEW_H

#include <uinta/types.h>
#include <uinta/text/text.h>
#include <uinta/text/font.h>

namespace uinta {

	class DebugController;

	struct DebugUiView {
		Text fpsLbl = initializeComponent("FPS:", 10, 0);
		Text fps = initializeComponent("0", 70, 0);

		Text tickLabel = initializeComponent("Tick:", 10, Font::LINE_HEIGHT);
		Text tick = initializeComponent("0", 70, Font::LINE_HEIGHT);

		Text cursorLabel = initializeComponent("Cursor:", 10, Font::LINE_HEIGHT * 2);
		Text cursor = initializeComponent("0x0", 100, Font::LINE_HEIGHT * 2);

		Text cursordLabel = initializeComponent("delta:", 40, Font::LINE_HEIGHT * 3);
		Text cursord = initializeComponent("0x0", 110, Font::LINE_HEIGHT * 3);

	private:
		static Text initializeComponent(const char *v, size_t x, size_t y) {
			Text t(v);
			t.setPositionPx(x, y);
			return t;
		}

	}; // struct DebugUiView

} // namespace uinta

#endif // UINTA_DEBUGUIVIEW_H
