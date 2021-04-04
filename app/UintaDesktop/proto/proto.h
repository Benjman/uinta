#ifndef UINTA_PROTOTYPE_H
#define UINTA_PROTOTYPE_H

#include <uinta/types.h>
#include <uinta/controller/controller.h>

namespace uinta {

	class Proto : public Controller {
	public:
		virtual void viewportChanged(size_t width, size_t height) {}

	}; // class DebugUi

} // namespace uinta

#endif // UINTA_PROTOTYPE_H
