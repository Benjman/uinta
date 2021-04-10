#include <uinta/render.h>

using namespace uinta;

void IRenderController::render() {
	if (_numRenderables == 0) return;
	for (auto renderable : _renderables) {
		if (renderable != nullptr) {
			// TODO batch rendering?
			renderable->render();
		}
	}
}
