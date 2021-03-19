#include <uinta/debug/debug_ui.h>

namespace uinta {

	DebugUiVao::DebugUiVao() :
			Vao(),
			vertexBuffer(Vbo::requestVbo(this, GL_ARRAY_BUFFER, GL_STATIC_DRAW, UI_VERTEX_BUFFER_SIZE)),
			indexBuffer(Vbo::requestVbo(this, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, UI_INDEX_BUFFER_SIZE)) {
		createAttribute(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	}

	DebugUi::DebugUi() : _element(UiElement(100, 100, 100, 100)) {
		updateBuffer();
	}

	void DebugUi::updateBuffer() {
		float_t vertices[UiElement::getVertexCount() * 2];
		uint32_t indices[UiElement::getIndexCount()];

		_element.generateMesh(vertices, indices);

		_vao.vertexBuffer.storeData(vertices, sizeof(vertices));
		_vao.indexBuffer.storeData(indices, sizeof(indices));
	}

}
