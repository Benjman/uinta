#include <uinta/debug/debug_controller.h>
#include <uinta/gl.h>
#include <uinta/render.h>
#include <uinta/shader.h>
#include <uinta/text.h>

namespace uinta::debuguicontroller {

	// TODO this should be in a DebugShader class
	static const char *vertexShader =
			"#version 300 es\n"
			"layout (location = 0) in lowp vec2 inPos;"
			"layout (location = 1) in lowp vec2 inUv;"
			"out lowp vec2 passUv;"
			"void main() {"
			"	gl_Position = vec4(inPos, 0.0, 1.0);"
			"	passUv = vec2(inUv.x, inUv.y);"
			"}\0";

	static const char *fragShader =
			"#version 300 es\n"
			"in lowp vec2 passUv;"
			"out lowp vec4 outColor;"
			"uniform sampler2D texture1;"
			"void main() {"
			"	outColor = vec4(texture(texture1, passUv).r);"
			"}\0";

}
using namespace uinta;
using namespace uinta::gl_state;
using namespace uinta::debuguicontroller;

DebugController::DebugController(Controller *parent)
		: BufferController(parent, KILOBYTES(5) * sizeof(GLfloat), KILOBYTES(2) * sizeof(GLuint)),
		  _shader(vertexShader, fragShader, Raw),
		  _font(Font::loadFont("/usr/share/fonts/noto/NotoSans-Regular.ttf")) {
}

void DebugController::initialize() {
	BufferController::initialize();

	initializeAttributes();
	generateMeshes();
	uploadBuffers();

	addRenderables();
}

void DebugController::addRenderables() {
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

void DebugController::generateMeshes() {
	size_t vPointer = 0, iPointer = 0, idxPointer = 0;
	for (auto child : getChildren()) {
		auto *controller = (TextController *) child; // TODO not everything is going to be a TextController. Need a way around this unsafe casting.
		Mesh *mesh = controller->getMesh();

		requestVBufferArena(controller->getVBufferLen(), &mesh->vBuffer, &mesh->vParentOffsetBytes);
		requestIBufferArena(controller->getIBufferLen(), &mesh->iBuffer, &mesh->iParentOffsetBytes);

		mesh->idxOffset = idxPointer;
		mesh->setIndexCount(controller->getICount());
		mesh->setOffset(iPointer);

		controller->populateMesh();

		vPointer += controller->getVBufferLen();
		iPointer += controller->getIBufferLen();
		idxPointer += controller->getMaxIdxCount();
	}
}

void DebugController::initializeAttributes() {
	// TODO there should be a DebugShader that manages these attributes
	vao->createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao->createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
}

DebugController::~DebugController() {
	delete _font;
}

void DebugController::render() {
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

	_shader.use();
	_font->bind();
	vao->bind();

	IRenderController::render();
}
