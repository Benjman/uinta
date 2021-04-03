#ifndef UINTA_DEBUGUIVIEW_H
#define UINTA_DEBUGUIVIEW_H

#include <uinta/types.h>
#include <uinta/text/text.h>

namespace uinta {

	class DebugUiController;

	struct DebugUiView {
		const Text fpsLbl;
		Text fps;

		DebugUiView();

	}; // struct DebugUiView

} // namespace uinta

#endif // UINTA_DEBUGUIVIEW_H


