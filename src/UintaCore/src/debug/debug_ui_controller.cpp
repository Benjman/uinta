#include <uinta/debug.h>
#include <uinta/debug/debug_ui_controller.h>
#include <uinta/gl_types.h>
#include <uinta/gl/gl_state.h>
#include <uinta/gl/gl_error.h>
#include <uinta/shader/vao.h>
#include <uinta/text/font.h>

using namespace uinta;
using namespace uinta::gl_state;

DebugUiController::DebugUiController(DebugController *parent)
		: BufferController(parent, KILOBYTES(5), KILOBYTES(2)),
		  _font(Font::loadFont("/usr/share/fonts/noto/NotoSans-Regular.ttf")) {
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

void DebugUiController::initialize() {
	BufferController::initialize();

	initializeTextControllers();
	uploadBuffers();

	addRenderables();
	_shader.initialize(*vao);
}

void DebugUiController::initializeTextControllers() {
	TextController *controllers[]{
			&_fps,
			&_fpsLabel,

			&_tick,
			&_tickLabel,
	};

	for (TextController *controller : controllers) {
		Mesh &mesh = *controller->getMesh();
		size_t iPointer = iIndex;
		initializeMeshBuffers(mesh, controller->getVBufferLen(), controller->getIBufferLen());

		mesh.idxOffset = idxIndex;
		idxIndex += controller->getMaxIdxCount();

		mesh.setIndexCount((GLsizei) controller->getICount());
		mesh.setOffset(iPointer);

		controller->populateMesh();
	}
}

void DebugUiController::addRenderables() {
	IRenderable *renderables[]{
			&_fps,
			&_fpsLabel,

			&_tick,
			&_tickLabel,
	};

	for (auto renderable : renderables) {
		addRenderable(renderable);
	}
}

DebugUiController::~DebugUiController() {
	delete _font;
}
