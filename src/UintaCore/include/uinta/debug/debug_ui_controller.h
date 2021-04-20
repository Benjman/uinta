#ifndef UINTA_DEBUG_UI_CONTROLLER_H
#define UINTA_DEBUG_UI_CONTROLLER_H

#include "camera_ui_controller.h"
#include "cursor_controller.h"
#include "debug_ui_shader.h"
#include "debug_ui_view.h"
#include "fps_controller.h"
#include "tick_controller.h"

#include <uinta/controller/buffer_controller.h>
#include <uinta/model/mesh.h>
#include <uinta/render/i_render_controller.h>

namespace uinta {

	class DebugController;
	class PerspectiveCamera;

	class DebugUiController : public BufferController, public IRenderController {
		const DebugUiView _view;
		const Font *_font{};

		DebugUiShader _shader;

		CameraUiController _camera;
		TextController _cameraLabel = TextController(this, (Text &) _view.cameraLabel, (Font *) _font);
		CursorController _cursor = CursorController(this, (Text &) _view.cursor, (Font *) _font);
		TextController _cursorLabel = TextController(this, (Text &) _view.cursorLabel, (Font *) _font);
		FpsController _fps = FpsController(this, (Text &) _view.fps, (Font *) _font);
		TextController _fpsLabel = TextController(this, (Text &) _view.fpsLbl, (Font *) _font);
		TickController _tick = TickController(this, (Text &) _view.tick, (Font *) _font);
		TextController _tickLabel = TextController(this, (Text &) _view.tickLabel, (Font *) _font);

	public:
		explicit DebugUiController(DebugController *parent, const PerspectiveCamera *pController);

		~DebugUiController();

		void render() override;

		void initialize() override;

		void initializeTextControllers();

		void addRenderables();

	}; // class DebugUiController

} // namespace uinta

#endif //UINTA_DEBUG_UI_CONTROLLER_H
