#include <mesh.hpp>
#include <quadtree.hpp>
#include <window.hpp>

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
    vec2 v0 = vec2(qt->topLeftBounds);
    vec2 v1 = vec2(v0 + vec2(0.0, LINE_SIZE));
    vec2 v2 = vec2(v0 + vec2(LINE_SIZE, 0.0));

    // bottom left
    vec2 v3 = vec2(qt->topLeftBounds.x, qt->bottomRightBounds.y - LINE_SIZE);
    vec2 v4 = vec2(v3 + vec2(0.0, LINE_SIZE));
    vec2 v5 = vec2(v3 + vec2(LINE_SIZE));

    // bottom right
    vec2 v6 = vec2(qt->bottomRightBounds - vec2(LINE_SIZE, 0.0));
    vec2 v7 = vec2(v6 + vec2(LINE_SIZE, 0.0));
    vec2 v8 = vec2(v6 + vec2(LINE_SIZE, -LINE_SIZE));

    // top right
    vec2 v9 = vec2(qt->bottomRightBounds.x, qt->topLeftBounds.y + LINE_SIZE);
    vec2 v10 = vec2(v9 - vec2(0, LINE_SIZE));
    vec2 v11 = vec2(v9 - vec2(LINE_SIZE));

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
    vertexBuffer[16] = v8.x;
    vertexBuffer[17] = v8.y;
    vertexBuffer[18] = v9.x;
    vertexBuffer[19] = v9.y;
    vertexBuffer[20] = v10.x;
    vertexBuffer[21] = v10.y;
    vertexBuffer[22] = v11.x;
    vertexBuffer[23] = v11.y;

    // adjust to screen size, and opengl ndc
    for (int i = 0; i < 24; i++) {
        if (i % 2 == 0) {
            vertexBuffer[i] /= window::width;
            vertexBuffer[i] = 2 * vertexBuffer[i] - 1;
        } else {
            vertexBuffer[i] /= window::height;
            vertexBuffer[i] = -2 * vertexBuffer[i] + 1;
        }
    }

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
    indexBuffer[12] = 6 + *indexOffset;
    indexBuffer[13] = 7 + *indexOffset;
    indexBuffer[14] = 10 + *indexOffset;
    indexBuffer[15] = 10 + *indexOffset;
    indexBuffer[16] = 11 + *indexOffset;
    indexBuffer[17] = 6 + *indexOffset;
    indexBuffer[18] = 9 + *indexOffset;
    indexBuffer[19] = 10 + *indexOffset;
    indexBuffer[20] = 0 + *indexOffset;
    indexBuffer[21] = 0 + *indexOffset;
    indexBuffer[22] = 1 + *indexOffset;
    indexBuffer[23] = 9 + *indexOffset;

    *indexOffset += 12;
    *vertexCount += 24;

    unsigned int vertCount = 0;
    if (qt->bottomLeft != nullptr) {
        generateMesh(qt->bottomLeft, &vertexBuffer[24], &indexBuffer[24], &vertCount, indexOffset);
        *vertexCount += vertCount;
        vertCount = 0;
    }

    if (qt->bottomRight != nullptr) {
        generateMesh(qt->bottomRight, &vertexBuffer[24], &indexBuffer[24], &vertCount, indexOffset);
        *vertexCount += vertCount;
        vertCount = 0;
    }

    if (qt->topLeft != nullptr) {
        generateMesh(qt->topLeft, &vertexBuffer[24], &indexBuffer[24], &vertCount, indexOffset);
        *vertexCount += vertCount;
        vertCount = 0;
    }

    if (qt->topRight != nullptr) {
        generateMesh(qt->topRight, &vertexBuffer[24], &indexBuffer[24], &vertCount, indexOffset);
        *vertexCount += vertCount;
        vertCount = 0;
    }
}
