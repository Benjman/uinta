#ifndef UINTA_I_RENDERABLE_H
#define UINTA_I_RENDERABLE_H

#include <uinta/types.h>
#include <uinta/gl_types.h>

namespace uinta {

	class IRenderable {
	public:
		virtual void render();

		virtual GLsizei getIndexCount() {
			return _indexCount;
		}

		void setIndexCount(GLsizei indexCount) {
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
		GLsizei _indexCount = 0;

	}; // class IRenderable

} // namespace uinta

#endif //UINTA_I_RENDERABLE_H
