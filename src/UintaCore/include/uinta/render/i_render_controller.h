#ifndef UINTA_I_RENDER_CONTROLLER_H
#define UINTA_I_RENDER_CONTROLLER_H

#include <vector>

namespace uinta {

	class IRenderable;

	class IRenderController {

		using Renderables = std::vector<IRenderable *>;

	public:

		Renderables _renderables;

		void addRenderable(IRenderable *renderable) {
			if (_storageSize + 1 >= _renderables.size()) {
				_storageSize += _storageStep;
				_renderables.resize(_storageSize);
			}
			_renderables.at(_numRenderables++) = renderable;
		}

		virtual void render();

	private:
		size_t _numRenderables = 0;
		size_t _storageSize = 0;
		size_t _storageStep = 10;
	}; // class IRenderController 

} // namespace uinta 



#endif //UINTA_I_RENDER_CONTROLLER_H
