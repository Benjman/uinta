#ifndef UINTA_DEBUG_UI_SHADER_H
#define UINTA_DEBUG_UI_SHADER_H

#include <uinta/shader/shader.h>

namespace uinta {

	class Vao;

	class DebugUiShader : public Shader {
	public:
		explicit DebugUiShader();

		void initialize(Vao &vao);

	}; // class DebugUiShader

} // namespace uinta

#endif //UINTA_DEBUG_UI_SHADER_H
