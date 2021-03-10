#ifndef UINTA_GL_STATE_H
#define UINTA_GL_STATE_H

#include "gl_types.h"

namespace uinta {

	struct gl_state {
		static vbo_id_t getBoundBuffer(vbo_target_t target);

		static void setBoundBuffer(vbo_target_t target, vbo_id_t id);
	};

}

#endif //UINTA_GL_STATE_H
