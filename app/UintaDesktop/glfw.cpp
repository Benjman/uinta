#include <uinta/os.h>

#include "glfw.h"

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
	std::cout << "Initializing for target " << uintaGetOsName() << std::endl;

	initializeGlfw(dto);
	if (dto.getStatus() != Initialized) {
		// TODO fatal logging
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return false;
	}
	std::cout << "Initialized GLFW version " << glfwGetVersionString() << std::endl;

	createWindow(dto);
	if (dto.getStatus() != WindowCreated) {
		// TODO fatal logging
		std::cerr << "Failed to create GLFW _window." << std::endl;
		return false;
	}
	std::cout << "Window created" << (dto.isHeadless() ? " (headless)" : "") << std::endl;
	setContext(dto);

	initializeGlad(dto);
	if (dto.getStatus() != GladInitialized) {
		// TODO fatal logging
		std::cerr << "Failed to initialize GLAD." << std::endl;
		return false;
	}
	std::cout << "GLAD initialized" << std::endl;

	printOSInfo();

	return true;
}

void uinta::glfw::createWindow(GlfwDto &dto) {
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
}

void uinta::glfw::framebufferSizeChangedHandler(GLFWwindow *window, int width, int height) {
	GlfwDto *&dto = internal::windows[window];
	if (dto == nullptr) {
		// TODO warn logging
		std::cerr << "Failed to respond to window resizing\n";
	}

	dto->setViewportSize(width, height);
	setGlViewport(*dto);
}

void uinta::glfw::glfwErrorHandler(int error, const char *description) {
	std::cerr << "GLFW Error:\n\t" << description << "\n\n";
}

void uinta::glfw::initializeGlfw(GlfwDto &dto) {
	if (!glfwInit()) {
		dto.setStatus(Error);
	}
	glfwSetErrorCallback(&glfwErrorHandler);
	dto.setStatus(Initialized);
}

void uinta::glfw::initializeGlad(GlfwDto &dto) {
	if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
		dto.setStatus(Error);
	}
	dto.setStatus(GladInitialized);
}

void uinta::glfw::printOSInfo() {
	std::cout << "\nOS INFO:" << std::endl;

	std::cout << "\tOpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\tMax threads: " << uintaGetHardwareConcurrencyCount() << std::endl;
	std::cout << std::endl;
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
