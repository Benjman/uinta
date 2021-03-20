#ifndef UINTA_I_RENDERABLE_H
#define UINTA_I_RENDERABLE_H

#include <uinta/types.h>

namespace uinta {

	class IRenderable {
	public:

		IRenderable(size_t offset = 0) : _offset(offset) {}

		virtual void render() = 0;

		virtual size_t getVertexCount() = 0;

		virtual size_t getIndexCount() {
			return _indexCount;
		}

		virtual size_t getOffset() {
			return _offset;
		}

		virtual void setOffset(size_t offset) {
			_offset = offset;
		}

	private:
		size_t _offset;
		size_t _indexCount;

	}; // class IRenderable 

} // namespace uinta

#endif //UINTA_I_RENDERABLE_H
