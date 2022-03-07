#include <mesh.hpp>
#include <quadtree.hpp>

const inline int LINE_SIZE = 2;

extern void generateMesh(const quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount, unsigned int *indexOffset) noexcept {
    if (qt == nullptr) {
        *vertexCount = 0;
        return;
    }

    float width = qt->bottomRightBounds.x - qt->topLeftBounds.x;
    float height = qt->bottomRightBounds.y - qt->topLeftBounds.y;

    /*
        Each corner is wound CCW (https://www.khronos.org/opengl/wiki/Face_Culling)
        Each corner has 3 vertices:

            v0  v2       v11 v10

            v1                v9



            v3                v8

            v4  v5        v6  v7

    */

    // top left
    vec2 v0 = qt->topLeftBounds;
    vec2 v1 = v0 + vec2(0.0, LINE_SIZE);
    vec2 v2 = v0 + vec2(LINE_SIZE, 0.0);

    // bottom left
    vec2 v3 = vec2(qt->topLeftBounds.x, qt->bottomRightBounds.y - LINE_SIZE);
    vec2 v4 = v3 + vec2(0.0, LINE_SIZE);
    vec2 v5 = v3 + vec2(LINE_SIZE);

    // bottom right
    vec2 v6 = qt->bottomRightBounds - vec2(LINE_SIZE, 0.0);
    vec2 v7 = v6 + vec2(LINE_SIZE, 0.0);
    vec2 v8 = v6 + vec2(LINE_SIZE, -LINE_SIZE);

    // top right
    vec2 v9 = qt->bottomRightBounds - vec2(LINE_SIZE, height - LINE_SIZE);
    vec2 v10 = v9 - vec2(0, LINE_SIZE);
    vec2 v11 = v9 - vec2(LINE_SIZE);

    vertexBuffer[0] = v0.x;
    vertexBuffer[1] = v0.y;
    vertexBuffer[2] = v1.x;
    vertexBuffer[3] = v1.y; vertexBuffer[4] = v2.x; vertexBuffer[5] = v2.y;
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
    vertexBuffer[16] = v8.x;
    vertexBuffer[17] = v8.y;
    vertexBuffer[18] = v9.x;
    vertexBuffer[19] = v9.y;
    vertexBuffer[20] = v10.x;
    vertexBuffer[21] = v10.y;
    vertexBuffer[22] = v11.x;
    vertexBuffer[23] = v11.y;

    indexBuffer[0] = 0 + *indexOffset;
    indexBuffer[1] = 4 + *indexOffset;
    indexBuffer[2] = 5 + *indexOffset;
    indexBuffer[3] = 5 + *indexOffset;
    indexBuffer[4] = 2 + *indexOffset;
    indexBuffer[5] = 0 + *indexOffset;
    indexBuffer[6] = 3 + *indexOffset;
    indexBuffer[7] = 4 + *indexOffset;
    indexBuffer[8] = 7 + *indexOffset;
    indexBuffer[9] = 7 + *indexOffset;
    indexBuffer[10] = 8 + *indexOffset;
    indexBuffer[11] = 3 + *indexOffset;
    indexBuffer[12] = 7 + *indexOffset;
    indexBuffer[13] = 10 + *indexOffset;
    indexBuffer[14] = 11 + *indexOffset;
    indexBuffer[15] = 11 + *indexOffset;
    indexBuffer[16] = 6 + *indexOffset;
    indexBuffer[17] = 7 + *indexOffset;
    indexBuffer[18] = 10 + *indexOffset;
    indexBuffer[19] = 0 + *indexOffset;
    indexBuffer[20] = 1 + *indexOffset;
    indexBuffer[21] = 0 + *indexOffset;
    indexBuffer[22] = 1 + *indexOffset;
    indexBuffer[23] = 9 + *indexOffset;

    *indexOffset += 24;

    // unsigned int vertCount = 0;
    // if (qt->bottomLeft != nullptr) {
    //     generateMesh(qt->bottomLeft, vertexBuffer, indexBuffer, &vertCount, indexOffset);
    //     *vertexCount += vertCount;
    //     vertCount = 0;
    // }
    //
    // if (qt->bottomRight != nullptr) {
    //     generateMesh(qt->bottomRight, vertexBuffer, indexBuffer, &vertCount, indexOffset);
    //     *vertexCount += vertCount;
    //     vertCount = 0;
    // }
    //
    // if (qt->topLeft != nullptr) {
    //     generateMesh(qt->topLeft, vertexBuffer, indexBuffer, &vertCount, indexOffset);
    //     *vertexCount += vertCount;
    //     vertCount = 0;
    // }
    //
    // if (qt->topRight != nullptr) {
    //     generateMesh(qt->topRight, vertexBuffer, indexBuffer, &vertCount, indexOffset);
    //     *vertexCount += vertCount;
    //     vertCount = 0;
    // }
}
