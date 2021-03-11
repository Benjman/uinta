#ifndef UINTA_VERTEX_ATTRIBUTE_H
#define UINTA_VERTEX_ATTRIBUTE_H

#include <uinta/gl/gl_types.h>

namespace uinta {

	class VertexAttribute {
		attrib_index_t _index;
		attrib_size_t _size;
		gl_type_t _type;
		attrib_normalize_t _normalized;
		attrib_stride_t _stride;
		const void *_offset;
		bool _enabled;

	public:
		VertexAttribute(attrib_index_t index, attrib_size_t size, gl_type_t type, attrib_normalize_t normalized,
						attrib_stride_t stride, const void *offset)
				: VertexAttribute(index, size, type, normalized, stride, offset, true) {}

		VertexAttribute(attrib_index_t index, attrib_size_t size, gl_type_t type, attrib_normalize_t normalized,
						attrib_stride_t stride, const void *offset, bool enable);

		void enable() const;

		void disable() const;

	}; // class VertexAttribute

} // namespace uinta

#endif //UINTA_VERTEX_ATTRIBUTE_H
