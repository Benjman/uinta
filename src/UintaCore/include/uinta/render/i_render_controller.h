#ifndef UINTA_I_RENDER_CONTROLLER_H
#define UINTA_I_RENDER_CONTROLLER_H

#include "i_renderable.h"

#include <vector>

namespace uinta {

	class IRenderable;

	class IRenderController : public IRenderable {

		using Renderables = std::vector<IRenderable *>;

	protected:
		Renderables _renderables;

	public:
		void addRenderable(IRenderable *renderable) {
			if (_storageSize + 1 >= _renderables.size()) {
				_storageSize += _storageStep;
				_renderables.resize(_storageSize);
			}
			_renderables.at(_numRenderables++) = renderable;
		}

		void render() override;

	private:
		// TODO why the fuck are we tracking it like this? Is pre-allocating really worth this mess?
		size_t _numRenderables = 0;
		size_t _storageSize = 0;
		size_t _storageStep = 10;
	}; // class IRenderController

} // namespace uinta



#endif //UINTA_I_RENDER_CONTROLLER_H
