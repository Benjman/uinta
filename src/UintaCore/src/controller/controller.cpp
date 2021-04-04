#include <uinta/controller/controller.h>

#include <algorithm> // std::find

using namespace uinta;

Controller::~Controller() {
	if (_parent) {
		std::vector<Controller *> &v = _parent->_children;
		v.erase(std::remove(v.begin(), v.end(), this), v.end());
	}
}
