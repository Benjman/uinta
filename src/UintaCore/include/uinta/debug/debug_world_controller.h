#ifndef UINTA_DEBUG_WORLD_CONTROLLER_H
#define UINTA_DEBUG_WORLD_CONTROLLER_H

#include "debug_world_shader.h"

#include <uinta/camera/icamera.h>
#include <uinta/controller/buffer_controller.h>
#include <uinta/model/mesh.h>
#include <uinta/render/i_render_controller.h>

namespace uinta {

	class DebugController;
	class DebugWorldController;

	class Floor : public Mesh {
		static const size_t COLS = 6;
		static const size_t ROWS = 6;
		static const size_t ELEMENTS_PER_VERT = 6; // 3d pos, color

		static const GLuint VERTS_PER_CELL = 4;
		static const GLuint INDICES_PER_CELL = 6;

		static constexpr float_t CELL_SIZE = 1.f;

	public:
		void initialize(DebugWorldController *controller);

	}; // class Floor

	class DebugWorldController : public BufferController, public IRenderController {
		const ICamera *_camera;
		DebugWorldShader _shader;
		Floor _floor;

	public:
		DebugWorldController(DebugController *parent, const ICamera *camera);

		void initialize() override;

		void render() override;

	}; // class DebugWorldController

} // namespace uinta

#endif //UINTA_DEBUG_WORLD_CONTROLLER_H
