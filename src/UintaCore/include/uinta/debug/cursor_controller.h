#ifndef UINTA_CURSOR_CONTROLLER_H
#define UINTA_CURSOR_CONTROLLER_H

#include <uinta/text/text_controller.h>

namespace uinta {

	class BufferController;
	class Font;
	class Text;

	class CursorController : public TextController {
		int16_t _x = 0, _y = 0;
		int16_t _dx = 0, _dy = 0;

		TextController &_delta;

	public:
		CursorController(BufferController *parent, Text &text, Font *font, TextController &delta);

		void update(const EngineState &state) override;

	}; // class MouseController

} // namespace uinta

#endif //UINTA_CURSOR_CONTROLLER_H
