#ifndef UINTA_DEBUG_WORLD_CONTROLLER_H
#define UINTA_DEBUG_WORLD_CONTROLLER_H

#include "debug_world_shader.h"

#include <uinta/controller/buffer_controller.h>
#include <uinta/render/i_render_controller.h>
#include <uinta/model/mesh.h>

namespace uinta {

	class CameraController;
	class DebugController;
	class DebugWorldController;

	class Floor : public Mesh {
	public:
		void initialize(DebugWorldController *controller);

	};

	class DebugWorldController : public BufferController, public IRenderController {
		const CameraController *_camera;
		DebugWorldShader _shader;
		Floor _floor;

	public:
		DebugWorldController(DebugController *parent, const CameraController *camera);

		void initialize() override;

		void render() override;

	}; // class DebugWorldController

} // namespace uinta

#endif //UINTA_DEBUG_WORLD_CONTROLLER_H
