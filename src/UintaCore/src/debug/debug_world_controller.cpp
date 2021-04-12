#include <uinta/debug/debug_controller.h>
#include <uinta/debug/debug_world_controller.h>
#include <uinta/shader/vao.h>
#include <uinta/camera/camera_controller.h>
#include <uinta/gl/gl_error.h>

using namespace uinta;
using namespace uinta::gl;

DebugWorldController::DebugWorldController(DebugController *parent, const CameraController *camera)
		: BufferController(parent, KILOBYTES(5), KILOBYTES(2)),
		_camera(camera) {
}

void DebugWorldController::initialize() {
	BufferController::initialize();

	_shader.initialize(glm::mat4(1), _camera->getProjectionMatrix(), _camera->getViewMatrix());
	_floor.initialize(this);

	uploadBuffers();

	vao->createAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
}

void DebugWorldController::render() {
	_shader.use();
	_shader._view.load(_camera->getViewMatrix());
	_shader._projection.load(_camera->getProjectionMatrix());

	glEnable(GL_DEPTH_TEST);
	glCheckError(GL_ENABLE);

	vao->bind();
	IRenderController::render();
}

void Floor::initialize(DebugWorldController *controller) {
	GLfloat vertices[] {
		-1.f, -1.f, 0.f,
		-1.f, 1.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, -1.f, 0.f
	};

	GLuint indices[] {
		0, 1, 3,
		1, 2, 3
	};

	size_t vLen = sizeof(vertices) / sizeof(GLfloat);
	size_t iLen = sizeof(indices) / sizeof(GLuint);
	controller->initializeMeshBuffers(*this, vLen, iLen);

	memcpy(vBuffer, vertices, sizeof(vertices));
	memcpy(iBuffer, indices, sizeof(indices));

	controller->addRenderable(this);

	setIndexCount(6);
	setOffset(0);
}
