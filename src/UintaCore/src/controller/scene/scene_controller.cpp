#include <uinta/controller/scene/scene_controller.h>
#include <uinta/gl/gl_error.h>
#include <uinta/gl/gl_state.h>
#include <uinta/shader.h>

#include <cstring>
#include <glm/ext/matrix_transform.hpp>

using namespace uinta;

SceneController::SceneController(Controller *parent, const ICamera *camera)
		: BufferController(parent, MEGABYTES(5),
						   MEGABYTES(3)),
						   _camera(camera) {
}

void SceneController::initialize() {
	BufferController::initialize();
	shader.initialize();
	shader._model.load(glm::mat4(1));
	shader._view.load(_camera->getViewMatrix());
	shader._projection.load(_camera->getProjectionMatrix());

	vao->createAttribute(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao->createAttribute(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
	vao->createAttribute(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));

	uploadBuffers();
}

void SceneController::render() {
	shader.use();
	vao->bind();

	glm::mat4 model(1.f);
//	model = glm::rotate(model, _runtime, glm::vec3(0.5f, 1.f, 0.f));

	shader._model.load(model);
	shader._view.load(_camera->getViewMatrix());
	shader._projection.load(_camera->getProjectionMatrix());

	glEnable(GL_DEPTH_TEST);
	glCheckError(GL_ENABLE);

	IRenderController::render();
}

void SceneController::update(const EngineState &state) {
	Controller::update(state);
	_runtime = state.runtime;
}
