#ifndef UINTA_DEBUG_WORLD_SHADER_H
#define UINTA_DEBUG_WORLD_SHADER_H

#include <uinta/shader.h>

namespace uinta {

	class DebugWorldShader : public Shader {
		friend class DebugWorldController;

		UniformMatrix4f _model = UniformMatrix4f("model");
		UniformMatrix4f _projection = UniformMatrix4f("projection");
		UniformMatrix4f _view = UniformMatrix4f("view");

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
