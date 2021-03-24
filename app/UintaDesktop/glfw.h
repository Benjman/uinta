#ifndef UINTA_GLFW_H
#define UINTA_GLFW_H

#include <glad/glad.h>
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

	};

	extern bool initialize(GlfwDto &dto);

	extern void createWindow(GlfwDto &dto);

	extern void framebufferSizeChangedHandler(GLFWwindow *window, int width, int height);

	extern void glfwErrorHandler(int error, const char *description);

	extern void initializeGlad(GlfwDto &dto);

	extern void initializeGlfw(GlfwDto &dto);

	extern void printInfo();

	extern void setContext(GlfwDto &dto);

	extern void setGlViewport(GlfwDto &dto);

	extern bool shouldClose(GlfwDto &dto);

	extern void terminate();

}

#endif //UINTA_GLFW_H
