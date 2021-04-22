#include <uinta/controller/buffer_controller.h>
#include <uinta/shader.h>
#include <uinta/model/mesh.h>

#include <iostream>

using namespace uinta;

void BufferController::initialize() {
	vao = Vao::requestVao();
	vbo = Vbo::requestVbo(vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, vSize);
	ibo = Vbo::requestVbo(vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, iSize);

	vMaxElements = vSize / sizeof(GLfloat);
	iMaxElements = iSize / sizeof(GLuint);

	Controller::initialize();
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

void BufferController::initializeMeshBuffers(Mesh &mesh) {
	reserveBuffer(&mesh.vBuffer, mesh.getVertexCount(), &mesh.vParentOffsetBytes,
				  &mesh.iBuffer, mesh.getIndexCount(), &mesh.iParentOffsetBytes);
}

void BufferController::reserveBuffer(GLfloat **pVBuffer, size_t pVLen, GLsizeiptr *pVOffsetBytes,
									 GLuint **pIBuffer, size_t pILen, GLsizeiptr *pIOffsetBytes) {
	if (pVLen) {
		if (vIndex + pVLen > vMaxElements) {
			std::cerr << "Exceeded max vertex elements. Ignoring reservation.\n";
			return;
		}
		*pVBuffer = &vBuffer[vIndex];
		*pVOffsetBytes = (GLsizeiptr) (vIndex * sizeof(GLfloat));
		vIndex += pVLen;
	}

	if (pILen) {
		if (iIndex + pILen > iMaxElements) {
			std::cerr << "Exceeded max index elements. Ignoring reservation and reverting vBuffer.\n";
			*pVBuffer = nullptr;
			vIndex -= pVLen;
			return;
		}
		*pIBuffer = &iBuffer[iIndex];
		*pIOffsetBytes = (GLsizeiptr) (iIndex * sizeof(GLuint));
		iIndex += pILen;
	}
}

void BufferController::uploadMesh(Mesh &mesh) {
	uploadMesh(mesh.vBuffer, (GLsizeiptr) (mesh.getVertexCount() * sizeof(GLfloat)), mesh.vParentOffsetBytes,
			   mesh.iBuffer, (GLsizeiptr) (mesh.getIndexCount() * sizeof(GLuint)), mesh.iParentOffsetBytes);
}
