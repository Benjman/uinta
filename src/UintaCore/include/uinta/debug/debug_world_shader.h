#ifndef UINTA_DEBUG_WORLD_SHADER_H
#define UINTA_DEBUG_WORLD_SHADER_H

#include <uinta/shader.h>

namespace uinta {

	class DebugWorldShader : public Shader {
		friend class DebugWorldController;

		UniformMatrix4fv _model = UniformMatrix4fv("model");
		UniformMatrix4fv _projection = UniformMatrix4fv("projection");
		UniformMatrix4fv _view = UniformMatrix4fv("view");

	public:
		DebugWorldShader();

		void loadUniforms() override {
			use();
			_model.storeLocation(_id);
			_projection.storeLocation(_id);
			_view.storeLocation(_id);
		}

		void initialize(glm::mat4 model, glm::mat4 projection, glm::mat4 view) {
			Shader::initialize();
			_model.load(model);
			_projection.load(projection);
			_view.load(view);
		}

		void initializeAttributes(Vao *vao) override;

	}; // class DebugUiShader

} // namespace uinta

#endif //UINTA_DEBUG_WORLD_SHADER_H
