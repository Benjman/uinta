#include "text_mesh_generator.h"

#include <uinta/text.h>

#include <regex>
#include <string>

using namespace uinta;

TextController::TextController(const Controller *parent, Text &text, Font *font) :
		Controller(parent),
		_text(&text),
		_font(font) {
	updateCounts();
}

size_t TextController::findWhitespaceCount() {
	std::regex regex("\\s");
	return std::distance(std::sregex_iterator(_text->getValue().begin(), _text->getValue().end(), regex),
						 std::sregex_iterator());
}

void TextController::initialize() {
	updateCounts();
	updateBuffers();
}

void TextController::updateBuffers() {
	_vBuffer = new float_t[_vertexCount * 4];
	_vBufferSize = _vertexCount * 4 * sizeof(float_t);

	_iBuffer = new uint32_t[_charCountRenderable * 6];
	_iBufferSize = _charCountRenderable * 6 * sizeof(uint32_t);

	TextMeshGenerator::generateMesh(*_text, *_font, _vBuffer, _iBuffer);
}
