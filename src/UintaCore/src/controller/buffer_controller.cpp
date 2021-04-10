#include <uinta/controller/buffer_controller.h>
#include <uinta/shader.h>
#include <uinta/model/mesh.h>

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

void BufferController::uploadMesh(GLfloat *pVBuffer, GLsizeiptr pVSize, GLsizeiptr pVOffset, GLuint *pIBuffer,
								  GLsizeiptr pISize, GLsizeiptr pIOffset) {
	vao->bind();
	vbo->storeData(pVBuffer, pVSize, pVOffset);
	ibo->storeData(pIBuffer, pISize, pIOffset);
}

void BufferController::initializeMeshBuffers(Mesh &mesh, size_t vLen, size_t iLen) {
	// TODO validate against buffer overflows
	mesh.vBuffer = &vBuffer[vIndex];
	mesh.iBuffer = &iBuffer[iIndex];

	mesh.vParentOffsetBytes = vIndex * sizeof(GLfloat);
	mesh.iParentOffsetBytes = iIndex * sizeof(GLuint);

	vIndex += vLen;
	iIndex += iLen;
}
