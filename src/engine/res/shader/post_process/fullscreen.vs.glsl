#version 330 core

// Oversized-triangle fullscreen pass. No VBO required; the three vertices
// (x = -1, 3, -1 ; y = -1, -1, 3) cover the entire [-1,1] NDC square, with
// the excess clipped by the rasterizer. UVs span [0,1] across the visible
// screen region.
//
// Usage: glDrawArrays(GL_TRIANGLES, 0, 3) with any empty VAO bound.

out vec2 pUv;

void main() {
  float x = float((gl_VertexID & 1) << 2) - 1.0;  // -1, 3, -1
  float y = float((gl_VertexID & 2) << 1) - 1.0;  // -1, -1, 3
  pUv = vec2((x + 1.0) * 0.5, (y + 1.0) * 0.5);
  gl_Position = vec4(x, y, 0.0, 1.0);
}
