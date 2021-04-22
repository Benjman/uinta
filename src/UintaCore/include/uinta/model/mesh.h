#ifndef UINTA_MESH_H
#define UINTA_MESH_H

#include <uinta/gl_types.h>
#include <uinta/types.h>
#include <uinta/render/i_renderable.h>

namespace uinta {

	struct Mesh : public IRenderable {

		// We are NOT the owner these buffers, do not deallocate.
		GLfloat *vBuffer{};
		GLuint *iBuffer{};

		GLsizeiptr vParentOffsetBytes = 0;
		GLsizeiptr iParentOffsetBytes = 0;

		size_t idxOffset = 0;

	}; // struct Mesh

} // namespace uinta

#endif //UINTA_MESH_H
