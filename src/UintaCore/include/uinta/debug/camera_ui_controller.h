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
		float_t _yaw = std::numeric_limits<float_t>::max();

		YawController(BufferController *parent, Text &text, Font *font, const PerspectiveCamera *camera)
				: TextController(parent, text, font, 6),
				  _camera(camera) {}

		void update(const EngineState &state) override;

	}; // YawController

	class ZoomController : public TextController {
		friend class CameraUiController;

		const PerspectiveCamera *_camera;
		float_t _zoom = std::numeric_limits<float_t>::max();

		ZoomController(BufferController *parent, Text &text, Font *font, const PerspectiveCamera *camera)
				: TextController(parent, text, font, 6),
				  _camera(camera) {}

		void update(const EngineState &state) override;

	}; // ZoomController

	class CameraUiController : public TextController {
		const PerspectiveCamera *_camera;
		YawController _yaw;
		ZoomController _zoom;
		float_t _pitch = 0.f;

	public:
		CameraUiController(BufferController *parent, Text &position, Font *font, Text &yaw, Text &zoom,
						   const PerspectiveCamera *camera)
				: TextController(parent, position, font, 6),
				  _camera(camera),
				  _yaw(parent, yaw, font, camera),
				  _zoom(parent, zoom, font, camera) {
		}

		void update(const EngineState &state) override;

		void initializeMeshBuffers(BufferController *buffer) override {
			TextController::initializeMeshBuffers(buffer);
			_yaw.initializeMeshBuffers(getParent());
			_zoom.initializeMeshBuffers(getParent());
		}

		[[nodiscard]] const YawController &getYaw() const { return _yaw; }
		[[nodiscard]] const ZoomController &getZoom() const { return _zoom; }

	}; // class CameraUiController

} // namespace uinta


#endif //UINTA_CAMERA_UI_CONTROLLER_H
