#ifndef UINTA_VAO_H
#define UINTA_VAO_H

#include <uinta/gl/gl_types.h>

#include <vector>

namespace uinta {

	class VertexAttribute;

	class Vao {
		friend class VertexAttribute;

		using AttributeCollection = std::vector<VertexAttribute *>;

	public:
		static void unbind();

		static Vao *requestVao();

	public:
		~Vao();

		void bind() const;

		VertexAttribute *createAttribute(attrib_index_t index, attrib_size_t size, gl_type_t type,
										attrib_normalize_t normalized, attrib_stride_t stride, const void *offset);

		void enableAllAttributes();

		void disableAllAttributes();

	private:
		AttributeCollection _attributes;
		vao_id_t _id = GL_ZERO;

		Vao();

		void removeAttribute(VertexAttribute *pAttribute);

	}; // class Vao

} // namespace uinta

#endif //UINTA_VAO_H
