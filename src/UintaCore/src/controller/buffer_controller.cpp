#include <uinta/controller/buffer_controller.h>
#include <uinta/shader.h>
#include <uinta/model/mesh.h>

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

void BufferController::initializeMeshBuffers(Mesh &mesh) {
	// TODO remove vLen and iLen. Use Mesh's IRenderable getVertexCount() and getIndexCount()
	reserveBuffer(&mesh.vBuffer, mesh.getVertexCount(), &mesh.vParentOffsetBytes, &mesh.iBuffer, mesh.getIndexCount(), &mesh.iParentOffsetBytes);
}

void BufferController::reserveBuffer(GLfloat **pVBuffer, size_t pVLen, GLsizeiptr *pVOffsetBytes,
									 GLuint **pIBuffer, size_t pILen, GLsizeiptr *pIOffsetBytes) {
	// TODO validate against buffer overflows
	if (pVLen) {
		*pVBuffer = &vBuffer[vIndex];
		*pVOffsetBytes = (GLsizeiptr) (vIndex * sizeof(GLfloat));
		vIndex += pVLen;
	}

	if (pILen) {
		*pIBuffer = &iBuffer[iIndex];
		*pIOffsetBytes = (GLsizeiptr) (iIndex * sizeof(GLuint));
		iIndex += pILen;
	}
}
