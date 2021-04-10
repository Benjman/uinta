#ifndef UINTA_SCENE_CONTROLLER_H
#define UINTA_SCENE_CONTROLLER_H

#include "scene_shader.h"

#include <uinta/controller/buffer_controller.h>
#include <uinta/render/i_render_controller.h>
#include <uinta/shader/shader.h>

namespace uinta {

	class CameraController;

	class SceneController : public BufferController, public IRenderController {
		const CameraController *_camera;
		SceneShader shader;
		float_t _runtime = 0.f;

	public:
		explicit SceneController(Controller *parent, const CameraController *camera);

		void initialize() override;

		void render() override;

		void update(const EngineState &state) override;

	};

}

#endif //UINTA_SCENE_CONTROLLER_H