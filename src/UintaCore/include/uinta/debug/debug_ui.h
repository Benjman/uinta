#ifndef UINTA_DEBUG_UI_H
#define UINTA_DEBUG_UI_H

#include <uinta/shader/vao.h>
#include <uinta/shader/vbo.h>
#include <uinta/types.h>
#include <uinta/ui.h>

#define UI_VERTEX_BUFFER_SIZE MEGABYTES(1)
#define UI_INDEX_BUFFER_SIZE KILOBYTES(512)

namespace uinta {

	struct DebugUiVao : Vao {
		Vbo vertexBuffer;
		Vbo indexBuffer;

		DebugUiVao();
	};

	class DebugUi {
		friend class DebugController;

		UiElement _element;
		DebugUiVao _vao;

		DebugUi();

	public:
		void updateBuffer();
	}; // class DebugUi

} // namespace uinta

#endif //UINTA_DEBUG_UI_H
