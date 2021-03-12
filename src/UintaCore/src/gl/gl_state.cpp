#include <uinta/gl/gl_state.h>

#include <unordered_map>

// TODO all these methods desperately need tests....

namespace uinta::internal {
	using BoundBufferMap = std::unordered_map<vbo_target_t, vbo_id_t>;
	static inline BoundBufferMap _boundBuffers{};

	static inline vao_id_t _boundVao = GL_ZERO;
}

using namespace uinta::internal;

uinta::vbo_id_t uinta::gl_state::getBoundBuffer(vbo_target_t target) {
	auto iterator = _boundBuffers.find(target);
	if (iterator != _boundBuffers.end()) {
		return iterator->second;
	}
	return INVALID_VBO_ID;
}

void uinta::gl_state::setBoundBuffer(vbo_target_t target, vbo_id_t id) {
	if (getBoundBuffer(target)) {
		_boundBuffers.at(target) = id;
	} else {
		_boundBuffers.insert(std::pair<vbo_target_t, vbo_id_t>(target, id));
	}
}

uinta::vao_id_t uinta::gl_state::getBoundVertexArray() {
	return _boundVao;
}

void uinta::gl_state::setBoundVertexArray(vao_id_t id) {
	_boundVao = id;
}

bool uinta::gl_state::isNotBoundVertexArrayElseSet(vao_id_t id) {
	bool isBound = id == getBoundVertexArray();
	if (!isBound) {
		setBoundVertexArray(id);
	}
	return !isBound;
}

bool uinta::gl_state::isNotBoundBufferElseSet(vbo_target_t target, vbo_id_t id) {
	bool isBound = id == getBoundBuffer(target);
	if (!isBound) {
		setBoundBuffer(target, id);
	}
	return !isBound;
}
