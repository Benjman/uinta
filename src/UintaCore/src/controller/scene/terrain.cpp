#include <uinta/controller/buffer_controller.h>
#include <uinta/controller/scene/terrain.h>
#include <uinta/controller/scene/scene_controller.h>

using namespace uinta;

Terrain::Terrain(SceneController *controller) : Model(controller, "models/terrain.obj") {
	controller->addRenderable(this);
}
