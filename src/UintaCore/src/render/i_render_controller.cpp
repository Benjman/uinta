#include <uinta/render.h>

namespace uinta {

	void IRenderController::render() {
		for (auto renderable : _renderables) {
			if (renderable != nullptr) {
				// TODO batch rendering?
				renderable->render();
			}
		}
	}

}
