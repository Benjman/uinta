#ifndef UINTA_DEBUG_UI_CONTROLLER_H
#define UINTA_DEBUG_UI_CONTROLLER_H

#include <uinta/controller/buffer_controller.h>
#include <uinta/debug/debug_ui_view.h>
#include <uinta/render/i_render_controller.h>
#include <uinta/text/text_controller.h>

namespace uinta {

	class Shader;
	class Font;

	class DebugUiController : public BufferController, IRenderController {

		class FpsTextController : public TextController {
			constexpr static const float_t INTERVAL = 0.5f;
			float_t timeToNextUpdate = INTERVAL;
			size_t frameCount = 0;

		public:
			FpsTextController(DebugUiController *parent, Text &text, Font *font, const DebugUiView &view) : TextController(parent, (Text &) view.fps, (Font *) font) {
				_maxChars = 3;
			}

			void update(const EngineState &state) override;

			void render() override {
				TextController::render();
				frameCount++;
			}

		};

		const Shader *_shader{};
		const Font *_font{};

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

		void addRenderables();

		void uploadMeshes();

	};

} // namespace uinta

#endif //UINTA_DEBUG_UI_CONTROLLER_H
