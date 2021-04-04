#ifndef UINTA_DEBUG_UI_CONTROLLER_H
#define UINTA_DEBUG_UI_CONTROLLER_H

#include <uinta/debug/debug_ui_view.h>

#include <uinta/controller/buffer_controller.h>
#include <uinta/text/text_controller.h>

namespace uinta {

	class Shader;
	class Font;

	class DebugUiController : public BufferController {

		struct FpsTextController : TextController {
			constexpr static const float_t INTERVAL = 0.25f;
			float_t timeToNextUpdate = INTERVAL;
			size_t frameCount = 0;

			FpsTextController(const DebugUiController *parent, Text &text, Font *font, const DebugUiView &view) : TextController(parent, (Text &) view.fps, (Font *) font) {
				_maxChars = 3;
			}

			void update(float_t dt) override;

			void initialize() override;

			void render() override {
				TextController::render();
				frameCount++;
			}
		};

		const Shader *_shader{};
		const Font *_font{};

		TextController **_controllers;
		size_t _controllerCount = 0;

		const DebugUiView _view = DebugUiView();

		FpsTextController _fps = FpsTextController(this, (Text &) _view.fps, (Font *) _font, _view);
		TextController _fpsLabel = TextController(this, (Text &) _view.fpsLbl, (Font *) _font);

		void generateMeshes();

	public:
		explicit DebugUiController(Controller *parent);

		~DebugUiController();

		void render() override;

		void initialize() override;

		void initializeBuffers() override;

		void initializeControllers();

		void update(float_t dt) override;

		void uploadMeshes();

	};

} // namespace uinta

#endif //UINTA_DEBUG_UI_CONTROLLER_H
