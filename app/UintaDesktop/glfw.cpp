#include "glfw.h"

#include <uinta/os.h>
#include <uinta/gl/gl_state.h>

#include <iostream>
#include <map>
#include <utility>

using namespace uinta::glfw;

namespace uinta::internal {
	using WindowMap = std::map<GLFWwindow *, GlfwDto *>;
	extern WindowMap windows;
}

uinta::internal::WindowMap uinta::internal::windows;

bool uinta::glfw::initialize(GlfwDto &dto) {
	initializeGlfw(dto);
	if (dto.getStatus() != Initialized) {
		// TODO fatal logging
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return false;
	}

	createWindow(dto);
	if (dto.getStatus() != WindowCreated) {
		// TODO fatal logging
		std::cerr << "Failed to create GLFW _window." << std::endl;
		return false;
	}

	initializeGlad(dto);
	if (dto.getStatus() != GladInitialized) {
		// TODO fatal logging
		std::cerr << "Failed to initialize GLAD." << std::endl;
		return false;
	}

	gl_state::setViewportSize(dto.getWidth(), dto.getHeight());

	std::cout << "Initialization complete" << std::endl;
	printInfo();

	return true;
}

void uinta::glfw::createWindow(GlfwDto &dto) {
	std::cout << "Creating " << (dto.isHeadless() ? "headless " : "") << "window... ";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, !dto.isHeadless());

	GLFWwindow *window = glfwCreateWindow(dto.getWidth(),
										  dto.getHeight(),
										  dto.getTitle().c_str(),
										  nullptr, nullptr);

	if (window == nullptr) {
		dto.setStatus(Error);
		return;
	}

	dto.setWindow(window);
	dto.setStatus(WindowCreated);
	internal::windows.insert(std::pair<GLFWwindow *, GlfwDto *>(window, &dto));

	glfwSetFramebufferSizeCallback(dto.getWindow(), framebufferSizeChangedHandler);
	setContext(dto);

	std::cout << "done" << std::endl;
}

void uinta::glfw::framebufferSizeChangedHandler(GLFWwindow *window, int width, int height) {
	GlfwDto *&dto = internal::windows[window];
	if (dto == nullptr) {
		// TODO warn logging
		std::cerr << "Failed to respond to window resizing\n";
	}

	dto->setViewportSize(width, height);
	setGlViewport(*dto);
	gl_state::setViewportSize(width, height);
}

void uinta::glfw::glfwErrorHandler(int error, const char *description) {
	std::cerr << "GLFW Error:\n\t" << description << "\n\n";
}

void uinta::glfw::initializeGlfw(GlfwDto &dto) {
	std::cout << "Initializing GLFW... ";
	if (!glfwInit()) {
		dto.setStatus(Error);
	}
	glfwSetErrorCallback(&glfwErrorHandler);
	dto.setStatus(Initialized);
	std::cout << "done" << std::endl;
}

void uinta::glfw::initializeGlad(GlfwDto &dto) {
	std::cout << "Initializing GLAD... ";
	if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
		dto.setStatus(Error);
	}
	dto.setStatus(GladInitialized);
	std::cout << "done" << std::endl;
}

void uinta::glfw::printInfo() {
	std::cout << "BUILD TYPE: " << (uintaIsDebugBuild() ? "Debug" : "Release") << std::endl;
	std::cout << "GLFW VERSION: " << glfwGetVersionString() << std::endl;
	std::cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "TARGET: " << uintaGetOsName() << std::endl;
	std::cout << "THREADS: " << uintaGetHardwareConcurrencyCount() << std::endl;
}

void uinta::glfw::setContext(GlfwDto &dto) {
	glfwMakeContextCurrent(dto.getWindow());
}

void uinta::glfw::setGlViewport(GlfwDto &dto) {
	glViewport(0, 0, dto.getWidth(), dto.getHeight());
}

bool uinta::glfw::shouldClose(GlfwDto &dto) {
	return glfwWindowShouldClose(dto.getWindow());
}

void uinta::glfw::terminate() {
	glfwTerminate();
}
