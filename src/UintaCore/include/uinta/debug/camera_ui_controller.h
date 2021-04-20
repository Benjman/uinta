#ifndef UINTA_CAMERA_UI_CONTROLLER_H
#define UINTA_CAMERA_UI_CONTROLLER_H

#include <uinta/text/text_controller.h>

namespace uinta {

	class BufferController;
	class PerspectiveCamera;
	class Font;
	class Text;

	class YawController : public TextController {
		friend class CameraUiController;

		const PerspectiveCamera *_camera;
		float_t _yaw = 0.f;

		YawController(BufferController *parent, Text &text, Font *font, const PerspectiveCamera *camera);

		void update(const EngineState &state) override;

	}; // YawController

	class ZoomController : public TextController {
		friend class CameraUiController;

		const PerspectiveCamera *_camera;
		float_t _zoom = 0.f;

		ZoomController(BufferController *parent, Text &text, Font *font, const PerspectiveCamera *camera);

		void update(const EngineState &state) override;

	}; // ZoomController

	class CameraUiController : public TextController {
		const PerspectiveCamera *_camera;
		YawController _yaw;
		ZoomController _zoom;
		float_t _pitch;

	public:
		CameraUiController(BufferController *parent, Text &position, Font *font, Text &yaw, Text &zoom,
						   const PerspectiveCamera *camera);

		void initialize() override;

		void update(const EngineState &state) override;

		void initializeMeshBuffers(BufferController *buffer) override;

		[[nodiscard]] const YawController &getYaw() const { return _yaw; }
		[[nodiscard]] const ZoomController &getZoom() const { return _zoom; }

	}; // class CameraUiController

} // namespace uinta


#endif //UINTA_CAMERA_UI_CONTROLLER_H
