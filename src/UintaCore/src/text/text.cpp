#include "text_mesh_generator.h"

#include <uinta/text/text.h>

#include <regex>

namespace uinta {

	void Text::generateMesh(float_t *data, uint32_t *indices, size_t *vertexCount, size_t *indexCount) {
		Text::generateMesh(*this, data, indices, vertexCount, indexCount);
	}

	void Text::generateMesh(Text &text, float_t *data, uint32_t *indices, size_t *vertexCount, size_t *indexCount) {
		TextMeshGenerator::generateMesh(text, data, indices, vertexCount, indexCount);
	}

	size_t Text::getNonSpaceCharacterCount() {
		return getNonSpaceCharacterCount(*this);
	}

	size_t Text::getNonSpaceCharacterCount(Text &text) {
		// TODO these need tests for each special character
		// TODO cache this value?
		std::regex rx("[a-zA-Z0-9!\"#$%&'()*+,-./:;<>=?@\\\\^_`{}~\\[\\]]"); // all ascii characters from 32-127
		return std::ptrdiff_t(std::distance(std::sregex_iterator(text._value.begin(), text._value.end(), rx),
											std::sregex_iterator()));
	}

}
