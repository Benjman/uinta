#ifndef UINTA_PROTOTYPE_H
#define UINTA_PROTOTYPE_H

#include <uinta/types.h>

namespace uinta {

	class Proto {
	public:
		virtual void update(float_t dt) = 0;

		virtual void render() = 0;

		virtual void viewportChanged(size_t width, size_t height) {}

	}; // class DebugUi 

} // namespace uinta 

#endif // UINTA_PROTOTYPE_H
