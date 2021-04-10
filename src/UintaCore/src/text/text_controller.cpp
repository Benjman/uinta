#include "text_mesh_generator.h"

#include <uinta/model/mesh.h>
#include <uinta/render.h>
#include <uinta/text.h>
#include <uinta/text/text_controller.h>

#include <iostream>


using namespace uinta;

TextController::TextController(BufferController *parent, Text &text, Font *font, size_t maxChars) :
		Controller(parent),
		_text(&text),
		_font(font),
		_mesh(new Mesh),
		_maxChars(maxChars) {
}

void TextController::initialize() {
	Controller::initialize();
	doUpdateMetadata();
}

void TextController::populateMesh() const {
	TextMeshGenerator::generateMesh(*_text, *_font, _mesh->vBuffer, _mesh->iBuffer, _mesh->idxOffset);
}

void TextController::doUpdateMetadata() {
	size_t charCount = _text->getCharCount();
	if (_maxChars == 0) {
		_maxChars = charCount;
	}
	if (charCount > _maxChars) {
		// Buffers are written up to _maxChars. Extra chars are ignored.
		std::cerr << "charCount exceeded _maxChars. Buffers are written up to _maxChars. Extra chars are ignored.\n";
	}
	_mesh->setIndexCount(getICount());
}

void TextController::render() {
	_mesh->render();
}

TextController::~TextController() {
	delete _mesh;
}

void TextController::setTextValue(const char *value, bool updateMetadata) {
	_text->_value = value;
	if (updateMetadata) {
		doUpdateMetadata();
	}
}

size_t TextController::getICount() const {
	return _text->getCharCount() * INDICES_PER_CHAR;
}

void TextController::uploadMesh(BufferController *controller) const {
	controller->uploadMesh(_mesh->vBuffer, getVBufferSize(), _mesh->vParentOffsetBytes, _mesh->iBuffer,
						   getIBufferSize(), _mesh->iParentOffsetBytes);
}
