#ifndef UINTA_CONTROLLER_H
#define UINTA_CONTROLLER_H

#include <uinta/types.h>
#include <uinta/engine_state.h>
#include <uinta/ecs.h>

#include <vector>

namespace uinta {

	class Controller {
		Controller *_parent;
		std::vector<Controller *> _children;

	protected:
		explicit Controller(Controller *parent = nullptr) : _parent(parent) {
			if (_parent) {
				_parent->_children.emplace_back(this);
			}
		}

	public:
		[[nodiscard]] virtual const Controller *getParent() const { return _parent; }
		[[nodiscard]] const std::vector<Controller *> &getChildren() const { return _children; }

		~Controller();

		virtual void initialize() {
			for (auto child : _children) {
				child->initialize();
			}
		}

		virtual void update(const EngineState &state) {
			for (auto child : _children) {
				child->update(state);
			}
		}

	}; // class Controller

} // namespace uinta

#endif //UINTA_CONTROLLER_H
