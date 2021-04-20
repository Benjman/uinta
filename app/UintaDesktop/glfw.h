#ifndef UINTA_GLFW_H
#define UINTA_GLFW_H

#include <uinta/input/input_manager_impl.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace uinta::glfw {

	enum GlfwStatus {
		Error,
		Initializing,
		Initialized,
		WindowCreated,
		GladInitialized
	};

	class GlfwDto {
		GlfwStatus _status = Initializing;
		GLFWwindow *_window = nullptr;
		int32_t _width = 0,
				_height = 0;
		std::string _title;
		bool _headless = false;
		bool _viewportChanged = false;

		size_t _numInputEvents = 0;
		InputEvent _inputEvents[15];

		float_t xCursorScroll = 0.f;
		float_t yCursorScroll = 0.f;
		int16_t xCursor = 0.f;
		int16_t yCursor = 0.f;
		bool cursorButtonsDown[8]{false};

	public:
		GlfwDto(int32_t width, int32_t height, std::string title)
				: _width(width), _height(height), _title(std::move(title)) {
		}

		[[nodiscard]] GlfwStatus getStatus() const {
			return _status;
		}

		void setStatus(GlfwStatus status) {
			_status = status;
		}

		[[nodiscard]] GLFWwindow *getWindow() const {
			return _window;
		}

		void setWindow(GLFWwindow *window) {
			_window = window;
		}

		int32_t &getWidth() {
			return _width;
		}

		int32_t &getHeight() {
			return _height;
		}

		void setViewportSize(int32_t width, int32_t height) {
			_viewportChanged = _width != width || _height != height;
			_width = width;
			_height = height;
		}

		std::string &getTitle() {
			return _title;
		}

		void setTitle(const std::string &title) {
			_title = title;
		}

		[[nodiscard]] bool isHeadless() const {
			return _headless;
		}

		void setHeadless(bool headless) {
			_headless = headless;
		}

		[[nodiscard]] bool isViewportChanged() const {
			return _viewportChanged;
		}

		void resetViewportChanged() {
			_viewportChanged = false;
		}

		void addInputEvent(InputEvent &event);

		void updateCursorScroll(float_t x, float_t y) {
			xCursorScroll = x;
			yCursorScroll = y;
		}

		void updateCursorPos(int16_t x, int16_t y) {
			xCursor = x;
			yCursor = y;
		}

		void updateCursorButton(cursor_code_t code, bool down) {
			cursorButtonsDown[code] = down;
		}

		void updateInputState(InputManagerImpl *inputManager) {
			for (size_t i = 0; i < _numInputEvents; i++) {
				inputManager->registerEvent(_inputEvents[i]);
			}
			_numInputEvents = 0;

			inputManager->setCursor(xCursor, yCursor, xCursorScroll, yCursorScroll, cursorButtonsDown);

			xCursorScroll = 0;
			yCursorScroll = 0;
		}

	};

	extern bool initialize(GlfwDto &dto);

	extern void createWindow(GlfwDto &dto);

	extern void glfwErrorHandler([[maybe_unused]] int error, const char *description);

	extern void initializeGlad(GlfwDto &dto);

	extern void initializeGlfw(GlfwDto &dto);

	extern void printInfo();

	extern void setContext(GlfwDto &dto);

	extern void setGlViewport(GlfwDto &dto);

	extern bool shouldClose(GlfwDto &dto);

	extern void terminate();

}

#endif //UINTA_GLFW_H
