#include <uinta/controller/buffer_controller.h>

#include <uinta/shader.h>

#include <iostream>

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

void BufferController::requestIBufferArena(size_t len, GLuint **ptr, size_t *offsetBytes) {
	if (len + iIndex > iSize) {
		std::cerr << "Buffer overflow. More space needed.\n";
		return;
	}
	*ptr = &iBuffer[iIndex];
	*offsetBytes = iIndex * sizeof(GLuint);
	iIndex += len;
}

void BufferController::requestVBufferArena(size_t len, GLfloat **ptr, size_t *offsetBytes) {
	if (len + vIndex > vSize) {
		std::cerr << "Buffer overflow. More space needed.\n";
		return;
	}
	*ptr = &vBuffer[vIndex];
	*offsetBytes = vIndex * sizeof(GLfloat);
	vIndex += len;
}
