#include "text_mesh_generator.h"

#include <uinta/text.h>

#include <uinta/text/text_controller.h>
#include <uinta/gl/gl_error.h>

#include <iostream>


using namespace uinta;

TextController::TextController(const BufferController *parent, Text &text, Font *font) :
		Controller(parent),
		_text(&text),
		_font(font) {
}

void TextController::initialize() {
	doUpdateMetadata();
}

void TextController::generateMesh(GLfloat *vBuffer, GLuint *iBuffer, size_t iOffset) const {
	TextMeshGenerator::generateMesh(*_text, *_font, vBuffer, iBuffer, iOffset);
}

void TextController::doUpdateMetadata() {
	_charCount = _text->getValue().size();
	if (_maxChars == 0) {
		_maxChars = _charCount;
	}
	if (_charCount > _maxChars) {
		// Buffers are written up to _maxChars. Extra chars are ignored.
		std::cerr << "_charCount exceeded _maxChars\n";
		_charCount = _maxChars;
	}
}

void TextController::render() {
	glDrawElements(GL_TRIANGLES, _charCount * INDICES_PER_CHAR, GL_UNSIGNED_INT, (void *)(_iOffset * sizeof(GLuint)));
	glCheckError(GL_DRAW_ELEMENTS);
}
