#include <uinta/render/i_renderable.h>
#include <uinta/gl.h>

using namespace uinta;
using namespace uinta::gl_state;

void IRenderable::render() {
	glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void *)(getOffset() * sizeof(GLuint)));
	glCheckError(GL_DRAW_ELEMENTS);
}
