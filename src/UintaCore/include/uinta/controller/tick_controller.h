#ifndef UINTA_TICK_CONTROLLER_H
#define UINTA_TICK_CONTROLLER_H

#include <uinta/text/text_controller.h>

namespace uinta {

	class BufferController;

	class TickController : public TextController {
		size_t _tick = 0;
		BufferController *_parent;

	public:
		TickController(BufferController *parent, Text &text, Font *font)
				: TextController(parent, text, font, 10),
				_parent(parent) {
		}

		void update(const EngineState &state) override;

	};

}

#endif //UINTA_TICK_CONTROLLER_H
