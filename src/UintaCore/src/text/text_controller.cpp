#include "text_mesh_generator.h"

#include <uinta/model/mesh.h>
#include <uinta/render.h>
#include <uinta/text.h>
#include <uinta/text/text_controller.h>

#include <iostream>


using namespace uinta;

TextController::TextController(BufferController *parent, Text &text, Font *font, GLsizei maxChars) :
		Controller(parent),
		_text(&text),
		_font(font),
		_mesh(new Mesh),
		_maxChars(maxChars),
		_parent(parent) {
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
		_maxChars = (GLsizei) charCount;
	}
	if (charCount > _maxChars) {
		// Buffers are written up to _maxChars. Extra chars are ignored.
		std::cerr << "charCount exceeded _maxChars. Buffers are written up to _maxChars. Extra chars are ignored.\n";
	}
	_mesh->setIndexCount((GLsizei) getICount());
}

void TextController::render() {
	_mesh->render();
}

TextController::~TextController() {
	delete _mesh;
}

void TextController::setTextValue(const char *value, bool updateMetadata) {
	_text->_value = value;
	if (_text->_value.length() > _maxChars) {
		_text->_value = _text->_value.substr(0, _maxChars);
	}
	if (updateMetadata) {
		doUpdateMetadata();
	}
}

GLuint TextController::getICount() const {
	return _text->getCharCount() * INDICES_PER_CHAR;
}

void TextController::uploadMesh() const {
	_parent->uploadMesh(_mesh->vBuffer, getVBufferSize(), _mesh->vParentOffsetBytes, _mesh->iBuffer,
						   getIBufferSize(), _mesh->iParentOffsetBytes);
}

void TextController::initializeMeshBuffers(BufferController *buffer) {
	_mesh->setVertexCount((GLsizei) getVBufferLen());
	_mesh->setIndexCount((GLsizei) getIBufferLen());
	size_t iPointer = buffer->getIIndex();
	buffer->initializeMeshBuffers(*_mesh, getMaxIdxCount());

	_mesh->setIndexCount((GLsizei) getICount());
	_mesh->setOffset(iPointer);

	populateMesh();
}
