#ifndef UINTA_CONTROLLER_H
#define UINTA_CONTROLLER_H

#include <uinta/types.h>

namespace uinta {

	class Controller {
		const Controller *_parent;

	protected:
		explicit Controller(const Controller *parent = nullptr) : _parent(parent) {}

	public:
		[[nodiscard]] const Controller *getParent() const {
			return _parent;
		}

		virtual void initialize() {}

		virtual void update(float_t dt) {}

		virtual void render() {}

	}; // class Controller

} // namespace uinta

#endif //UINTA_CONTROLLER_H
