#ifndef UINTA_TERRAIN_H
#define UINTA_TERRAIN_H

#include <uinta/model/model.h>

namespace uinta {

	class SceneController;

	class Terrain : public Model {
	public:
		explicit Terrain(SceneController *controller);

	}; // class Terrain

} // namespace uinta

#endif //UINTA_TERRAIN_H
