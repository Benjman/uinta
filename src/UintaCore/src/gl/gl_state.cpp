#include <uinta/gl/gl_state.h>

#include <unordered_map>

// TODO all these methods desperately need tests....

namespace uinta::gl_state {
	using BoundBufferMap = std::unordered_map<vbo_target_t, vbo_id_t>;
	extern BoundBufferMap _boundBuffers;

	extern vao_id_t _boundVao;
	extern program_id_t _programInUse;
}

using namespace uinta::gl_state;

BoundBufferMap uinta::gl_state::_boundBuffers{};
uinta::vao_id_t uinta::gl_state::_boundVao = GL_ZERO;
uinta::program_id_t uinta::gl_state::_programInUse = GL_ZERO;

uinta::vbo_id_t uinta::getBoundBuffer(vbo_target_t target) {
	auto iterator = _boundBuffers.find(target);
	if (iterator != _boundBuffers.end()) {
		return iterator->second;
	}
	return INVALID_VBO_ID;
}

void uinta::setBoundBuffer(vbo_target_t target, vbo_id_t id) {
	if (getBoundBuffer(target)) {
		_boundBuffers.at(target) = id;
	} else {
		_boundBuffers.insert(std::pair<vbo_target_t, vbo_id_t>(target, id));
	}
}

uinta::vao_id_t uinta::getBoundVertexArray() {
	return _boundVao;
}

void uinta::setBoundVertexArray(vao_id_t id) {
	_boundVao = id;
}

bool uinta::isNotBoundVertexArrayElseSet(vao_id_t id) {
	bool isBound = id == getBoundVertexArray();
	if (!isBound) {
		setBoundVertexArray(id);
	}
	return !isBound;
}

bool uinta::isNotBoundBufferElseSet(vbo_target_t target, vbo_id_t id) {
	bool isBound = id == getBoundBuffer(target);
	if (!isBound) {
		setBoundBuffer(target, id);
	}
	return !isBound;
}

uinta::program_id_t uinta::getCurrentProgramId() {
	return _programInUse;
}

void uinta::setCurrentProgramId(uinta::program_id_t id) {
	_programInUse = id;
}

bool uinta::isProgramNotInUseElseSet(uinta::program_id_t id) {
	bool inUse = id == getCurrentProgramId();
	if (!inUse) {
		setCurrentProgramId(id);
	}
	return !inUse;
}
