#ifndef UINTA_SCENE_SHADER_H
#define UINTA_SCENE_SHADER_H

#include <uinta/shader/shader.h>
#include <uinta/shader/uniform.h>

namespace uinta {

	class SceneShader : public Shader {
		friend class SceneController;

		UniformMatrix4fv _model = UniformMatrix4fv("model");
		UniformMatrix4fv _projection = UniformMatrix4fv("projection");
		UniformMatrix4fv _view = UniformMatrix4fv("view");

	public:
		SceneShader();

		void loadUniforms() override {
			use();
			_model.storeLocation(_id);
			_projection.storeLocation(_id);
			_view.storeLocation(_id);
		}
	};

} // namespace uinta

#endif //UINTA_SCENE_SHADER_H
