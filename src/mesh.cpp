#include <mesh.hpp>
#include <quadtree.hpp>
#include <window.hpp>

const inline auto LINE_SIZE = 4;

extern void generateMesh(const quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount, unsigned int *indexCount, unsigned int *indexOffset) noexcept {
    if (qt == nullptr)
        return;

    /*
        Triangles are wound CCW (https://www.khronos.org/opengl/wiki/Face_Culling)
        Each corner has 2 vertices:

            v0              v6
                v1      v7



                v3      v5
            v2              v4
    */

    auto v0 = vec2(qt->topLeftBounds);
    auto v1 = v0 + LINE_SIZE;
    auto v2 = vec2(qt->topLeftBounds.x, qt->bottomRightBounds.y);
    auto v3 = v2 + vec2(LINE_SIZE, - LINE_SIZE);
    auto v4 = vec2(qt->bottomRightBounds);
    auto v5 = v4 - LINE_SIZE;
    auto v6 = vec2(qt->bottomRightBounds.x, qt->topLeftBounds.y);
    auto v7 = v6 + vec2(-LINE_SIZE, LINE_SIZE);

    vertexBuffer[0] = v0.x;
    vertexBuffer[1] = v0.y;
    vertexBuffer[2] = v1.x;
    vertexBuffer[3] = v1.y;
    vertexBuffer[4] = v2.x;
    vertexBuffer[5] = v2.y;
    vertexBuffer[6] = v3.x;
    vertexBuffer[7] = v3.y;
    vertexBuffer[8] = v4.x;
    vertexBuffer[9] = v4.y;
    vertexBuffer[10] = v5.x;
    vertexBuffer[11] = v5.y;
    vertexBuffer[12] = v6.x;
    vertexBuffer[13] = v6.y;
    vertexBuffer[14] = v7.x;
    vertexBuffer[15] = v7.y;
    auto localVertexCount = 16u;

    indexBuffer[0] = 2 + *indexOffset;
    indexBuffer[1] = 1 + *indexOffset;
    indexBuffer[2] = 0 + *indexOffset;
    indexBuffer[3] = 2 + *indexOffset;
    indexBuffer[4] = 3 + *indexOffset;
    indexBuffer[5] = 1 + *indexOffset;
    indexBuffer[6] = 5 + *indexOffset;
    indexBuffer[7] = 3 + *indexOffset;
    indexBuffer[8] = 2 + *indexOffset;
    indexBuffer[9] = 4 + *indexOffset;
    indexBuffer[10] = 5 + *indexOffset;
    indexBuffer[11] = 2 + *indexOffset;
    indexBuffer[12] = 4 + *indexOffset;
    indexBuffer[13] = 6 + *indexOffset;
    indexBuffer[14] = 5 + *indexOffset;
    indexBuffer[15] = 6 + *indexOffset;
    indexBuffer[16] = 7 + *indexOffset;
    indexBuffer[17] = 5 + *indexOffset;
    indexBuffer[18] = 6 + *indexOffset;
    indexBuffer[19] = 0 + *indexOffset;
    indexBuffer[20] = 1 + *indexOffset;
    indexBuffer[21] = 6 + *indexOffset;
    indexBuffer[22] = 1 + *indexOffset;
    indexBuffer[23] = 7 + *indexOffset;
    auto localIndexCount = 24u;
    *indexOffset += 8u;

    if (qt->bottomLeft && qt->bottomLeft->isActive())
        generateMesh(qt->bottomLeft , &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset);
    if (qt->bottomRight && qt->bottomRight->isActive())
        generateMesh(qt->bottomRight, &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset);
    if (qt->topLeft && qt->topLeft->isActive())
        generateMesh(qt->topLeft, &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset);
    if (qt->topRight && qt->topRight->isActive())
        generateMesh(qt->topRight, &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset);

    *vertexCount += localVertexCount;
    *indexCount += localIndexCount;
}
