#include <uinta/controller/buffer_controller.h>

#include <uinta/shader.h>

using namespace uinta;

void BufferController::initialize() {
	Controller::initialize();
	vao = Vao::requestVao();
	vbo = Vbo::requestVbo(vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, vSize);
	ibo = Vbo::requestVbo(vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, iSize);
}

BufferController::~BufferController() {
	if (vao != nullptr) {
		vao->bind();
	}
	delete ibo;
	delete vbo;
	delete vao;
	delete[] vBuffer;
	delete[] iBuffer;
}

void BufferController::uploadMesh(GLfloat *pVBuffer, size_t pVSize, size_t pVOffset, GLuint *pIBuffer, size_t pISize,
								  GLuint pIOffset) {
	vao->bind();
	vbo->storeData(pVBuffer, pVSize, pVOffset);
	ibo->storeData(pIBuffer, pISize, pIOffset);
}
