#ifndef UINTA_MESH_H
#define UINTA_MESH_H

#include <uinta/gl/gl_types.h>
#include <uinta/types.h>
#include <uinta/render/i_renderable.h>

namespace uinta {

	struct Mesh : public IRenderable {

		// We are NOT the owner these buffers, do not deallocate.
		GLfloat *vBuffer{};
		GLuint *iBuffer{};

		size_t vParentOffsetBytes = 0;
		size_t iParentOffsetBytes = 0;

		size_t idxOffset = 0;

	}; // struct Mesh

} // namespace uinta

#endif //UINTA_MESH_H
