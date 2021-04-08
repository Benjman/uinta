#ifndef UINTA_SCENE_CONTROLLER_H
#define UINTA_SCENE_CONTROLLER_H

#include "scene_shader.h"

#include <uinta/controller/buffer_controller.h>
#include <uinta/render/i_render_controller.h>
#include <uinta/shader/shader.h>

namespace uinta {

	class SceneController : public BufferController, public IRenderController {
		SceneShader shader;
		float_t _running = 0.f;

	public:
		explicit SceneController(Controller *parent);

		void initialize() override;

		void render() override;

		void update(const EngineState &state) override;

	};

}

#endif //UINTA_SCENE_CONTROLLER_H
