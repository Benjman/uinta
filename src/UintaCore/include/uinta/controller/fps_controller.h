#ifndef UINTA_FPS_CONTROLLER_H
#define UINTA_FPS_CONTROLLER_H

#include <uinta/text/text_controller.h>

namespace uinta {

	class BufferController;

	class FpsController : public TextController {
		constexpr static const float_t INTERVAL = 0.5f;
		float_t _timeToNextUpdate = INTERVAL;
		size_t _frameCount = 0;
		size_t _curFps = 0;
		BufferController *_parent;

	public:
		FpsController(BufferController *parent, Text &text, Font *font);

		void update(const EngineState &state) override;

		void render() override {
			TextController::render();
			_frameCount++;
		}

	};

}

#endif //UINTA_FPS_CONTROLLER_H
