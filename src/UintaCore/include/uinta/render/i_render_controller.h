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
			_renderables.emplace_back(renderable);
		}

		void render() override;

	}; // class IRenderController

} // namespace uinta



#endif //UINTA_I_RENDER_CONTROLLER_H
