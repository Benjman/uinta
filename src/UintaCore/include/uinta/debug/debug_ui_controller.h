#ifndef UINTA_DEBUGUICONTROLLER_H
#define UINTA_DEBUGUICONTROLLER_H

#include "debug_ui_view.h"

#include <uinta/text/text_controller.h>
#include <uinta/controller/controller.h>

#include <uinta/data/arr_utils.h>

namespace uinta {

	class Shader;
	class Vao;
	class Vbo;

	class DebugUiController : public Controller {
		struct FpsTextController : TextController {
			constexpr static const float_t INTERVAL = 1.f;
			float_t timeToNextUpdate = INTERVAL;
			size_t frameCount = 0;
			size_t shift = 0;

			FpsTextController(const DebugUiController *parent, Text &text, Font *font, const DebugUiView &view) : TextController(parent, (Text &) view.fps, (Font *) font) {}

			void update(float_t dt) override;

			void frameRendered() {
				frameCount++;
			}

			void setAndDoShift(size_t pShift) {
				shift = pShift;
				doShift();
			}

			void doShift() {
			}
		};


		const Shader *_shader;
		const Font *_font;

		const DebugUiView _view = DebugUiView();
		FpsTextController _fps = FpsTextController(this, (Text &) _view.fps, (Font *) _font, _view);
		TextController _fpsLbl = TextController(this, (Text &) _view.fpsLbl, (Font *) _font);

		Vao *_vao{};
		Vbo *_vertexBuffer{};
		Vbo *_indexBuffer{};

		size_t _vertexBufferSize = 0;
		size_t _indexBufferSize = 0;

	public:
		DebugUiController();

		void initialize() override;

		void update(float_t dt) override;

		void render() override;

		~DebugUiController();

		[[nodiscard]] const Shader *getShader() const {
			return _shader;
		}

		[[nodiscard]] const DebugUiView &getView() const {
			return _view;
		}

		[[nodiscard]] const Font *getFont() const {
			return _font;
		}

		void initFps();

		void initializeControllers();

		void initializeBuffers();
	}; // class DebugUiController

} // namespace uinta

#endif // UINTA_DEBUGUICONTROLLER_H

