#ifndef UINTA_SCENE_SHADER_H
#define UINTA_SCENE_SHADER_H

#include <uinta/shader/shader.h>
#include <uinta/shader/uniform.h>

namespace uinta {

	class SceneShader : public Shader {
		friend class SceneController;

		UniformMatrix4f _model = UniformMatrix4f("model");
		UniformMatrix4f _projection = UniformMatrix4f("projection");
		UniformMatrix4f _view = UniformMatrix4f("view");

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
