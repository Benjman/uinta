#ifndef UINTA_VERTEX_ATTRIBUTE_H
#define UINTA_VERTEX_ATTRIBUTE_H

#include <uinta/gl/gl_types.h>

namespace uinta {

	class VertexAttribute {
		friend class Vao;

		attrib_index_t _index;
		bool _enabled;

	public:
		void enable(bool force = false);

		void disable(bool force = false);

	private:
		VertexAttribute(attrib_index_t index, attrib_size_t size, gl_type_t type,
						attrib_normalize_t normalized, attrib_stride_t stride, const void *offset, bool enable = true);

	}; // class VertexAttribute

} // namespace uinta

#endif //UINTA_VERTEX_ATTRIBUTE_H
