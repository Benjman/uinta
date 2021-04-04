#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-slicing"

#include "uinta/debug/debug_ui_controller.h"

#include <uinta/shader.h>
#include <uinta/text.h>
#include <uinta/gl.h>

namespace uinta::debuguicontroller {

	static const char *vertexShader =
			"#version 300 es\n"
			"layout (location = 0) in lowp vec2 inPos;"
			"layout (location = 1) in lowp vec2 inTexCoord;"
			"out lowp vec2 passTexCoord;"
			"void main() {"
			"	gl_Position = vec4(inPos, 0.0, 1.0);"
			"	passTexCoord = vec2(inTexCoord.x, inTexCoord.y);"
			"}";

	static const char *fragShader =
			"#version 300 es\n"
			"in lowp vec2 passTexCoord;"
			"out lowp vec4 outColor;"
			"uniform sampler2D texture1;"
			"void main() {"
			"	outColor = vec4(texture(texture1, passTexCoord).r);"
			"}";

}
using namespace uinta;
using namespace uinta::gl_state;
using namespace uinta::debuguicontroller;

DebugUiController::DebugUiController(Controller *parent) : BufferController(parent, KILOBYTES(5) * sizeof(GLfloat), KILOBYTES(2) * sizeof(GLuint)),
														   _shader(Shader::createShader(vertexShader, fragShader, Raw)),
														   _font(Font::loadFont(
																   "/usr/share/fonts/noto/NotoSans-Regular.ttf")) {
	_controllerCount = 2;
	_controllers = new TextController *[_controllerCount]{ // TODO this is ugly af
			&_fps,
			&_fpsLabel,
	};

	vBuffer = new GLfloat[vSize / sizeof(GLfloat)];
	iBuffer = new GLuint[iSize / sizeof(GLuint)];
}

void DebugUiController::FpsTextController::update(float_t dt) {
	Controller::update(dt);
	timeToNextUpdate -= dt;
	if (timeToNextUpdate <= 0) {
		const std::string &fpsStr = std::to_string((size_t) std::ceil(frameCount * (1 / INTERVAL)));

		if (fpsStr != _text->getValue()) {
			setValue(fpsStr.c_str());

			DebugUiController *parent = ((DebugUiController *) getParent());
			generateMesh(&parent->vBuffer[_vOffset], &parent->iBuffer[_iOffset], _idxOffset);
			parent->uploadMesh(&parent->vBuffer[_vOffset], getVBufferSize(), _vOffset, &parent->iBuffer[_iOffset], getIBufferSize(), _iOffset);
		}

		frameCount = 0;
		timeToNextUpdate += INTERVAL;
	}
}

void DebugUiController::FpsTextController::initialize() {
	TextController::initialize();
}

void DebugUiController::initialize() {
	BufferController::initialize();
	initializeControllers();
	initializeBuffers();
	generateMeshes();
	uploadMeshes();
}

void DebugUiController::initializeControllers() {
	for (size_t i = 0; i < _controllerCount; i++) {
		_controllers[i]->initialize();
	}
}

void DebugUiController::generateMeshes() {
	size_t vPointer = 0, iPointer = 0, idxPointer = 0;

	for (size_t i = 0; i < _controllerCount; i++) {
		TextController *controller = _controllers[i];

		controller->_vOffset = vPointer;
		controller->_iOffset = iPointer;
		controller->_idxOffset = idxPointer;

		controller->generateMesh(&vBuffer[vPointer], &iBuffer[iPointer], idxPointer);

		vPointer += controller->getVBufferLen();
		iPointer += controller->getIBufferLen();
		idxPointer += controller->getIdxCount();
	}
}

void DebugUiController::initializeBuffers() {
	vao->createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao->createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
}

void DebugUiController::uploadMeshes() {
	uploadMesh(vBuffer, vSize, 0, iBuffer, iSize, 0);
}

DebugUiController::~DebugUiController() {
	delete _font;
	delete _shader;
	delete[] _controllers;
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

	for (size_t i = 0; i < _controllerCount; i++) {
		_controllers[i]->render();
	}
}

void DebugUiController::update(float_t dt) {
	Controller::update(dt);
	_fps.update(dt);
}

#pragma clang diagnostic pop
