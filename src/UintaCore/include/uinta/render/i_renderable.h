#ifndef UINTA_I_RENDERABLE_H
#define UINTA_I_RENDERABLE_H

#include <uinta/types.h>

namespace uinta {

	class IRenderable {
	public:
		virtual void render();

		virtual size_t getIndexCount() {
			return _indexCount;
		}

		void setIndexCount(size_t indexCount) {
			_indexCount = indexCount;
		}

		virtual size_t getOffset() {
			return _offset;
		}

		virtual void setOffset(size_t offset) {
			_offset = offset;
		}

	protected:
		size_t _offset = 0;
		size_t _indexCount = 0;

	}; // class IRenderable

} // namespace uinta

#endif //UINTA_I_RENDERABLE_H
