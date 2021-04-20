#include <uinta/debug.h>
#include <uinta/debug/debug_ui_controller.h>
#include <uinta/gl_types.h>
#include <uinta/gl/gl_state.h>
#include <uinta/gl/gl_error.h>
#include <uinta/shader/vao.h>
#include <uinta/text/font.h>

using namespace uinta;
using namespace uinta::gl_state;

DebugUiController::DebugUiController(DebugController *parent, const PerspectiveCamera *cameraController)
		: BufferController(parent, KILOBYTES(10), KILOBYTES(5)),
		  _font(Font::loadFont("/usr/share/fonts/noto/NotoSans-Regular.ttf")),
		  _camera(this, (Text &) _view.cameraPitch, (Font *) _font, (Text &) _view.cameraYaw, (Text &) _view.cameraZoom, cameraController) {
}

void DebugUiController::initialize() {
	BufferController::initialize();

	initializeTextControllers();
	uploadBuffers();

	addRenderables();
	_shader.initialize(*vao);
}

void DebugUiController::initializeTextControllers() {
	_camera.initializeMeshBuffers(this);
	_cameraLabel.initializeMeshBuffers(this);
	_cursor.initializeMeshBuffers(this);
	_cursorLabel.initializeMeshBuffers(this);
	_fps.initializeMeshBuffers(this);
	_fpsLabel.initializeMeshBuffers(this);
	_tick.initializeMeshBuffers(this);
	_tickLabel.initializeMeshBuffers(this);
}

void DebugUiController::addRenderables() {
	addRenderable(&_camera);
	addRenderable(&_cameraLabel);
	addRenderable((IRenderable*) &_camera.getYaw());
	addRenderable((IRenderable*) &_camera.getZoom());
	addRenderable(&_cursor);
	addRenderable(&_cursorLabel);
	addRenderable(&_fps);
	addRenderable(&_fpsLabel);
	addRenderable(&_tick);
	addRenderable(&_tickLabel);
}

void DebugUiController::render() {
	if (!isActiveElseSet(ENABLE, GL_BLEND, GL_TRUE)) {
		glEnable(GL_BLEND);
		glCheckError(GL_ENABLE);
	}

	if (!isActiveElseSet(BLEND_FUNC, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCheckError(GL_BLEND_FUNC);
	}

	if (isActiveElseSet(ENABLE, GL_DEPTH_TEST, GL_FALSE)) {
		glDisable(GL_DEPTH_TEST);
		glCheckError(GL_ENABLE);
	}

	vao->bind();
	_font->bind();
	_shader.use();

	IRenderController::render();
}

DebugUiController::~DebugUiController() {
	delete _font;
}
