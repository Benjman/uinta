#include <uinta/controller/scene/scene_controller.h>
#include <uinta/shader.h>
#include <uinta/gl/gl_error.h>
#include <uinta/gl/gl_state.h>

#include <cstring>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace uinta;

SceneController::SceneController(Controller *parent)
		: BufferController(parent, KILOBYTES(5),
						   KILOBYTES(2)) {
}

void SceneController::initialize() {
	BufferController::initialize();
	shader.initialize();
	shader._model.load(glm::mat4(1));
	shader._view.load(glm::mat4(1));
	shader._projection.load(glm::mat4(1));

	GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	};

	const GLuint indices[]{
			0, 1, 3,
			1, 2, 3
	};

	std::memcpy(vBuffer, vertices, sizeof(vertices));
	std::memcpy(iBuffer, indices, sizeof(indices));

	vao->createAttribute(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
	vao->createAttribute(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

	uploadBuffers();
}

void SceneController::render() {
	shader.use();
	vao->bind();

	glm::mat4 model(1.f);
	glm::mat4 view(1.f);
	glm::mat4 projection(1.f);

	model = glm::rotate(model, _running, glm::vec3(0.5f, 1.f, 0.f));
	view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));
	projection = glm::perspective(glm::radians(45.f), gl_state::getViewportAspectRatio(), 0.1f, 100.f);

	shader._model.load(model);
	shader._view.load(view);
	shader._projection.load(projection);

	glEnable(GL_DEPTH_TEST);
	glCheckError(GL_ENABLE);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glCheckError(GL_DRAW_ARRAYS);
}

void SceneController::update(const EngineState &state) {
	Controller::update(state);
	_running = state.runtime;
}
