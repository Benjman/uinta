#include <uinta/gl.h>
#include <uinta/ui/ui_element.h>

#include <glm/glm.hpp>
#include <algorithm>
#include <cstring>

using namespace uinta;

inline void applyAspectRatio(glm::vec4 &vec) {
	float_t aspect = gl_state::getViewportAspectRatio();
	if (aspect > 1) {
		// horizontal
		vec.x *= aspect;
		vec.z *= aspect;
	} else {
		// vertical
		vec.y *= aspect;
		vec.w *= aspect;
	}
}

inline glm::vec4 toUnitSpace(glm::uvec4 &dimensions) {
	return glm::vec4(
			dimensions.x / UI_BASE_SIZE,
			dimensions.y / UI_BASE_SIZE,
			dimensions.z / UI_BASE_SIZE,
			dimensions.w / UI_BASE_SIZE
	);
}

inline void scale(UiElement &element, glm::vec4 &vec) {
	vec.x *= element.getScale();
	vec.y *= element.getScale();
	vec.z *= element.getScale();
	vec.w *= element.getScale();
}

void UiElement::generateMesh(float_t *data, uint32_t *indices) {
	glm::uvec4 dimensions(_position.x, _position.y, _size.x, _size.y);

	if (_parent != nullptr) {
		dimensions.x += _parent->_position.x;
		dimensions.y += _parent->_position.y;
	}

	glm::vec4 unitSpace = toUnitSpace(dimensions);
	applyAspectRatio(unitSpace);
	scale(*this, unitSpace);

	/*
	 v0  _____  v3
		|     |
		|     |
		|_____|
	 v1         v2
	 */
	const float_t buf[8]{
			TO_GL_NDC_X(unitSpace.x), TO_GL_NDC_Y(unitSpace.y),
			TO_GL_NDC_X(unitSpace.x), TO_GL_NDC_Y(unitSpace.w),
			TO_GL_NDC_X(unitSpace.z), TO_GL_NDC_Y(unitSpace.w),
			TO_GL_NDC_X(unitSpace.z), TO_GL_NDC_Y(unitSpace.y)
	};
	const uint32_t idx[6]{
			0, 1, 3,
			1, 2, 3
	};
	std::memcpy(data, buf, sizeof(buf));
	std::memcpy(indices, idx, sizeof(idx));
}

void UiElement::render() {
	glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (const void *) getOffset());
	glCheckError(GL_DRAW_ELEMENTS);
}

void UiElement::removeChild(UiElement *element) {
	auto iterator = std::find(_children.begin(), _children.end(), element);
	if (iterator != _children.end()) {
		_children.erase(iterator);
		element->_parent = nullptr;
	}
}

void UiElement::addChild(UiElement *element) {
	_children.push_back(element);
	element->_parent = this;
	if (element->_size.x == 0) {
		element->_size.x = _size.x;
	}
	if (element->_size.y == 0) {
		element->_size.y = _size.y;
	}
}
