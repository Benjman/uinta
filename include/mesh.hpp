#ifndef UINTA_GENERATORS_H
#define UINTA_GENERATORS_H

struct quad;

extern void generateMesh(const quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount, unsigned int *indexOffset) noexcept;

#endif // UINTA_GENERATORS_H
