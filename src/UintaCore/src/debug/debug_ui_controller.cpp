#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-slicing"

#include <uinta/debug/debug_ui_controller.h>
#include <uinta/gl.h>
#include <uinta/render.h>
#include <uinta/shader.h>
#include <uinta/text.h>

namespace uinta::debuguicontroller {

	static const char *vertexShader =
			"#version 300 es\n"
			"layout (location = 0) in lowp vec2 inPos;"
			"layout (location = 1) in lowp vec2 inUv;"
			"out lowp vec2 passUv;"
			"void main() {"
			"	gl_Position = vec4(inPos, 0.0, 1.0);"
			"	passUv = vec2(inUv.x, inUv.y);"
			"}";

	static const char *fragShader =
			"#version 300 es\n"
			"in lowp vec2 passUv;"
			"out lowp vec4 outColor;"
			"uniform sampler2D texture1;"
			"void main() {"
			"	outColor = vec4(texture(texture1, passUv).r);"
			"}";

}
using namespace uinta;
using namespace uinta::gl_state;
using namespace uinta::debuguicontroller;

DebugUiController::DebugUiController(Controller *parent)
		: BufferController(parent, KILOBYTES(5) * sizeof(GLfloat), KILOBYTES(2) * sizeof(GLuint)),
		  _shader(Shader::createShader(vertexShader, fragShader, Raw)),
		  _font(Font::loadFont( "/usr/share/fonts/noto/NotoSans-Regular.ttf")) {
	vBuffer = new GLfloat[vSize / sizeof(GLfloat)];
	iBuffer = new GLuint[iSize / sizeof(GLuint)];
}

void DebugUiController::initialize() {
	BufferController::initialize();

	initializeBuffers();
	generateMeshes();
	uploadMeshes();

	addRenderables();
}

void DebugUiController::addRenderables() {
	IRenderable *renderables[] {
		&_fps,
		&_fpsLabel,

		&_tick,
		&_tickLabel,
	};

	for (auto renderable : renderables) {
		addRenderable(renderable);
	}
}

void DebugUiController::generateMeshes() {
	size_t vPointer = 0, iPointer = 0, idxPointer = 0;
	for (auto child : getChildren()) {
		auto *controller = (TextController *) child; // TODO not everything is going to be a TextController. Need a way around this unsafe casting. (Maybe this should be in TextController?)
		Mesh *mesh = controller->getMesh();

		mesh->vBuffer = &vBuffer[vPointer];
		mesh->iBuffer = &iBuffer[iPointer];

		mesh->vParentOffsetBytes = vPointer * sizeof(GLfloat);
		mesh->iParentOffsetBytes = iPointer * sizeof(GLuint);
		mesh->idxOffset = idxPointer;

		mesh->setIndexCount(controller->getICount());
		mesh->setOffset(iPointer);

		controller->populateMesh();

		vPointer += controller->getVBufferLen();
		iPointer += controller->getIBufferLen();
		idxPointer += controller->getMaxIdxCount();
	}
}

void DebugUiController::initializeBuffers() {
	// TODO there should be a TextShader that manages these attributes
	vao->createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao->createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
}

void DebugUiController::uploadMeshes() {
	uploadMesh(vBuffer, vSize, 0, iBuffer, iSize, 0);
}

DebugUiController::~DebugUiController() {
	delete _font;
	delete _shader;
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

	_shader->use();
	_font->bind();

	IRenderController::render();
}

#pragma clang diagnostic pop
