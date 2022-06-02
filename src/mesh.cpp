#include <uinta/mesh.hpp>
#include <uinta/quadtree.hpp>

const inline auto LINE_SIZE = 4;
const inline auto LINE_SIZE_HALF = LINE_SIZE / 2.0f;

extern void generateMesh(const Quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount, unsigned int *indexCount, unsigned int *indexOffset, const unsigned int width, const unsigned int height) noexcept {
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

    auto v0 = glm::vec2(qt->topLeftBounds - LINE_SIZE_HALF);
    auto v1 = glm::vec2(qt->topLeftBounds + LINE_SIZE_HALF);
    auto v2 = glm::vec2(qt->topLeftBounds.x - LINE_SIZE_HALF, qt->bottomRightBounds.y + LINE_SIZE_HALF);
    auto v3 = glm::vec2(qt->topLeftBounds.x + LINE_SIZE_HALF, qt->bottomRightBounds.y - LINE_SIZE_HALF);
    auto v4 = glm::vec2(qt->bottomRightBounds + LINE_SIZE_HALF);
    auto v5 = glm::vec2(qt->bottomRightBounds - LINE_SIZE_HALF);
    auto v6 = glm::vec2(qt->bottomRightBounds.x + LINE_SIZE_HALF, qt->topLeftBounds.y - LINE_SIZE_HALF);
    auto v7 = glm::vec2(qt->bottomRightBounds.x - LINE_SIZE_HALF, qt->topLeftBounds.y + LINE_SIZE_HALF);

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

    // adjust vertices to screen size, and opengl ndc
    for (int i = 0; i < 24; i++) {
        if (i % 2 == 0) {
            vertexBuffer[i] /= width;
            vertexBuffer[i] = 2 * vertexBuffer[i] - 1;
        } else {
            vertexBuffer[i] /= height;
            vertexBuffer[i] = -2 * vertexBuffer[i] + 1;
        }
    }

    if (qt->bottomLeft && qt->bottomLeft->isActive())
        generateMesh(qt->bottomLeft , &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset, width, height);
    if (qt->bottomRight && qt->bottomRight->isActive())
        generateMesh(qt->bottomRight, &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset, width, height);
    if (qt->topLeft && qt->topLeft->isActive())
        generateMesh(qt->topLeft, &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset, width, height);
    if (qt->topRight && qt->topRight->isActive())
        generateMesh(qt->topRight, &vertexBuffer[localVertexCount], &indexBuffer[localIndexCount], &localVertexCount, &localIndexCount, indexOffset, width, height);

    *vertexCount += localVertexCount;
    *indexCount += localIndexCount;
}

const MeshAttrib* findMeshAttrib(MeshAttribType type, const std::unordered_map<MeshAttribType, MeshAttrib>* attribs) {
    if (attribs->find(type) == attribs->end())
        return nullptr;
    return &attribs->at(type);
}
