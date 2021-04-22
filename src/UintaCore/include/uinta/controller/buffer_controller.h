#ifndef UINTA_BUFFER_CONTROLLER_H
#define UINTA_BUFFER_CONTROLLER_H

#include "controller.h"
#include <uinta/gl_types.h>

namespace uinta {

	class Vao;
	class Vbo;
	class Mesh;

	class BufferController : public Controller {
	protected:
		Vao *vao{};
		Vbo *vbo{};
		Vbo *ibo{}; // indices

		GLfloat *vBuffer = nullptr;
		GLuint *iBuffer = nullptr;

		GLsizeiptr vSize = 0;
		GLsizeiptr iSize = 0;

		size_t vMaxElements = 0;
		size_t iMaxElements = 0;

		size_t vIndex = 0;
		size_t iIndex = 0;
		size_t idxIndex = 0;

	public:
		BufferController(Controller *parent, GLsizeiptr vSize, GLuint iSize)
				: Controller(parent), vSize(vSize),
				  iSize(iSize) {
			vBuffer = new GLfloat[vSize / sizeof(GLfloat)];
			iBuffer = new GLuint[iSize / sizeof(GLuint)];
		}

		~BufferController();

		void initialize() override;

		void uploadMesh(Mesh &mesh);

		void uploadMesh(GLfloat *pVBuffer, GLsizeiptr pVSize, GLsizeiptr pVOffset, GLuint *pIBuffer,
								  GLsizeiptr pISize, GLsizeiptr pIOffset = 0);

		void uploadBuffers() {
			uploadMesh(vBuffer, vSize, 0, iBuffer, iSize, 0);
		}

		void initializeMeshBuffers(Mesh &mesh);

		void reserveBuffer(GLfloat **pVBuffer, size_t pVLen, GLsizeiptr *pVOffsetBytes,
											 GLuint **pIBuffer, size_t pILen, GLsizeiptr *pIOffsetBytes);

		[[nodiscard]] GLfloat *getVBuffer() const { return vBuffer; }
		[[nodiscard]] GLuint *getIBuffer() const { return iBuffer; }
		[[nodiscard]] GLsizeiptr getVSize() const { return vSize; }
		[[nodiscard]] GLsizeiptr getISize() const { return iSize; }
		[[nodiscard]] size_t getVMaxElements() const { return vMaxElements; }
		[[nodiscard]] size_t getIMaxElements() const { return iMaxElements; }
		[[nodiscard]] size_t getVIndex() const { return vIndex; }
		[[nodiscard]] size_t getIIndex() const { return iIndex; }
		[[nodiscard]] size_t getIdxIndex() const { return idxIndex; }
		void addIdxIndex(size_t amt) { idxIndex += amt; }

	}; // class BufferController

} // namespace uinta

#endif //UINTA_BUFFER_CONTROLLER_H
