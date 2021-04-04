#ifndef UINTA_BUFFER_CONTROLLER_H
#define UINTA_BUFFER_CONTROLLER_H

#include "controller.h"
#include <uinta/gl/gl_types.h>

namespace uinta {

	class Vao;

	class Vbo;

	class BufferController : public Controller {
	protected:
		Vao *vao{};
		Vbo *vbo{};
		Vbo *ibo{}; // indices

		GLfloat *vBuffer = nullptr;
		GLuint *iBuffer = nullptr;

		size_t vSize = 0;
		size_t iSize = 0;

		virtual void initializeBuffers() = 0;

	public:
		BufferController(Controller *parent, size_t vSize, GLuint iSize) : Controller(parent), vSize(vSize),
																		   iSize(iSize) {}

		~BufferController();

		void initialize() override;

		void uploadMesh(GLfloat *pVBuffer, size_t pVSize, size_t pVOffset, GLuint *pIBuffer, size_t pISize,
						GLuint pIOffset = 0);

	}; // class BufferController

} // namespace uinta

#endif //UINTA_BUFFER_CONTROLLER_H
