#ifndef UINTA_GL_STATE_H
#define UINTA_GL_STATE_H

#include "gl_types.h"

namespace uinta {

	extern vbo_id_t getBoundBuffer(vbo_target_t target);

	extern void setBoundBuffer(vbo_target_t target, vbo_id_t id);

	extern vao_id_t getBoundVertexArray();

	extern void setBoundVertexArray(vao_id_t id);

	/***
	 * @return Whether the currently bound vertex array is NOT id. If not, set internally bound vertex array to id.
	 */
	extern bool isNotBoundVertexArrayElseSet(vao_id_t id);

	/***
	 * @return Whether the id is NOT currently bound to the target. If not, set internally bound buffer to id for the target.
	 */
	extern bool isNotBoundBufferElseSet(vbo_target_t target, vbo_id_t id);

	extern program_id_t getCurrentProgramId();

	extern void setCurrentProgramId(program_id_t id);

	/***
	 * @return Whether id is currently NOT in use. If not, set internally bound program to id.
	 */
	extern bool isProgramNotInUseElseSet(program_id_t id);

}

#endif //UINTA_GL_STATE_H
