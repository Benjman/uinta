#ifndef UINTA_DEBUG_CONTROLLER_H
#define UINTA_DEBUG_CONTROLLER_H

#include <uinta/controller/buffer_controller.h>
#include <uinta/controller/fps_controller.h>
#include <uinta/controller/tick_controller.h>
#include <uinta/debug/debug_ui_view.h>
#include <uinta/render/i_render_controller.h>
#include <uinta/shader/shader.h>
#include <uinta/text/text_controller.h>

namespace uinta {

	class Font;

	class DebugController : public BufferController, IRenderController {
		const Shader _shader;
		const Font *_font{};

		const DebugUiView _view;

		FpsController _fps = FpsController(this, (Text &) _view.fps, (Font *) _font);
		TextController _fpsLabel = TextController(this, (Text &) _view.fpsLbl, (Font *) _font);

		TickController _tick = TickController(this, (Text &) _view.tick, (Font *) _font);
		TextController _tickLabel = TextController(this, (Text &) _view.tickLabel, (Font *) _font);

		void initializeTextController();

		void addRenderables();

		void initializeAttributes();

	public:
		explicit DebugController(Controller *parent);

		~DebugController();

		void render() override;

		void initialize() override;
	};

} // namespace uinta

#endif //UINTA_DEBUG_CONTROLLER_H
