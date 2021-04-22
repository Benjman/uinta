#include <uinta/model.h>
#include <uinta/controller/buffer_controller.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "NullDereferences"

using namespace Assimp;

namespace uinta::internal {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;
	};

	void calcMeshCount(size_t *count, aiNode *node) {
		*count += node->mNumMeshes;
		for (size_t i = 0; i < node->mNumChildren; i++) {
			calcMeshCount(count, node->mChildren[i]);
		}
	}

	Mesh *processMesh(aiMesh *aiMesh, BufferController *buffer) {
		size_t numComponents = 2; // pos, norm
		size_t componentSize = 3; // x, y, z
		Vertex vertices[aiMesh->mNumVertices];
		GLuint indices[aiMesh->mNumFaces * 3]; // assumes triangles

		for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
			Vertex vertex{};
			vertex.position.x = aiMesh->mVertices[i].x;
			vertex.position.y = aiMesh->mVertices[i].y;
			vertex.position.z = aiMesh->mVertices[i].z;
			if (aiMesh->HasNormals()) {
				vertex.normal.x = aiMesh->mNormals[i].x;
				vertex.normal.y = aiMesh->mNormals[i].y;
				vertex.normal.z = aiMesh->mNormals[i].z;
			}
			vertices[i] = vertex;
		}

		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (size_t ii = 0; ii < face.mNumIndices; ii++) {
				indices[i * 3 + ii] = face.mIndices[ii];
			}
		}

		Mesh *mesh = new Mesh;
		mesh->setVertexCount((GLsizei) aiMesh->mNumVertices * numComponents * componentSize);
		mesh->setIndexCount((GLsizei)(sizeof(indices) / sizeof(GLuint)));
		buffer->initializeMeshBuffers(*mesh);

		size_t index = 0;
		for (auto vertex : vertices) {
			mesh->vBuffer[index++] = vertex.position.x;
			mesh->vBuffer[index++] = vertex.position.y;
			mesh->vBuffer[index++] = vertex.position.z;
			mesh->vBuffer[index++] = vertex.normal.x;
			mesh->vBuffer[index++] = vertex.normal.y;
			mesh->vBuffer[index++] = vertex.normal.z;
		}

		memcpy(mesh->iBuffer, indices, sizeof(indices));
		buffer->uploadMesh(*mesh);

		return mesh;
	}

	void processNode(aiNode *node, const aiScene *scene, BufferController *buffer, Mesh **meshes, size_t meshIndex = 0) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
			meshes[meshIndex++] = processMesh(aiMesh, buffer);
		}
		for (size_t i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene, buffer, meshes, meshIndex);
		}
	}

}

using namespace uinta;
using namespace uinta::internal;

Model Model::loadModel(const char *path, BufferController *buffer) {
	Importer importer;

	const aiScene *scene = importer.ReadFile(path,
											 aiProcess_Triangulate |
											 aiProcess_FlipUVs |
											 aiProcess_JoinIdenticalVertices);

	if (!scene
		|| scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
		|| !scene->mRootNode) {
		// TODO error handling
	}

	size_t meshCount = 0;
	calcMeshCount(&meshCount, scene->mRootNode);

	Mesh **meshes = new Mesh*[meshCount];

	processNode(scene->mRootNode, scene, buffer, meshes);

	return Model(meshes, meshCount);
}

Model::~Model() {
	for (size_t i = 0; i < _numChildren; i++) {
		delete _children[i];
	}
}

Model::Model(Mesh **children, size_t numChildren) : _children(children), _numChildren(numChildren) {
}

void Model::render() {
	for (size_t i = 0; i < _numChildren; i++) {
		_children[i]->render();
	}
}

#pragma clang diagnostic pop
