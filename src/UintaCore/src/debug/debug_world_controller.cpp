#include <uinta/colors.h>
#include <uinta/debug/debug_controller.h>
#include <uinta/debug/debug_world_controller.h>
#include <uinta/gl/gl_error.h>

using namespace uinta;
using namespace uinta::gl;

DebugWorldController::DebugWorldController(DebugController *parent, const ICamera *camera)
		: BufferController(parent, KILOBYTES(5), KILOBYTES(2)),
		  _camera(camera) {
}

void DebugWorldController::initialize() {
	BufferController::initialize();

	_shader.initialize(glm::mat4(1), _camera->getProjectionMatrix(), _camera->getViewMatrix());
	_shader.initializeAttributes(vao);
	_floor.initialize(this);

	uploadBuffers();
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
	setIndexCount(INDICES_PER_CELL * ROWS * COLS);
	setVertexCount(VERTS_PER_CELL * ELEMENTS_PER_VERT * ROWS * COLS);
	setOffset(0);

	controller->initializeMeshBuffers(*this);
	controller->addRenderable(this);

	const glm::vec3 colors[]{ColorGrey300, ColorGrey500};
	glm::vec3 color;

	float_t xCursor, xEnd, yEnd;
	float_t yCursor = ROWS * CELL_SIZE * -.5f;

	for (GLuint y = 0, i = 0, iOff; y < ROWS; y++, i++) {
		xCursor = ROWS * CELL_SIZE * -.5f;
		yEnd = yCursor + CELL_SIZE;

		for (GLuint x = 0; x < COLS; x++, i++) {
			xEnd = xCursor + CELL_SIZE;
			color = colors[i % 2];

			GLfloat vertices[]{
					xCursor, 0.f, yCursor, color.r, color.g, color.b,
					xCursor, 0.f, yEnd, color.r, color.g, color.b,
					xEnd, 0.f, yEnd, color.r, color.g, color.b,
					xEnd, 0.f, yCursor, color.r, color.g, color.b,
			};
			memcpy(&vBuffer[(y * COLS * ELEMENTS_PER_VERT + x * ELEMENTS_PER_VERT) * VERTS_PER_CELL],
				   vertices, sizeof(vertices));

			iOff = (y * COLS + x) * VERTS_PER_CELL;
			GLuint indices[]{
					0 + iOff, 1 + iOff, 3 + iOff,
					1 + iOff, 2 + iOff, 3 + iOff,
			};
			memcpy(&iBuffer[(y * COLS + x) * INDICES_PER_CELL], indices, sizeof(indices));

			xCursor += CELL_SIZE;
		}

		yCursor += CELL_SIZE;
	}
}
