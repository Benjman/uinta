#include <uinta/model.h>
#include <uinta/controller/buffer_controller.h>
#include <uinta/data/arr_utils.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "NullDereferences"

using namespace Assimp;

namespace uinta::internal {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	void calcMeshCount(size_t *count, aiNode *node) {
		*count += node->mNumMeshes;
		for (size_t i = 0; i < node->mNumChildren; i++) {
			calcMeshCount(count, node->mChildren[i]);
		}
	}

	Mesh *processMesh(aiMesh *aiMesh, BufferController *buffer, aiMaterial *pMaterial) {
		size_t numComponents = 3; // pos, norm, color
		size_t componentSize = 3; // x, y, z
		GLsizei idxCount = (GLsizei) aiMesh->mNumFaces * 3;
		Vertex vertices[aiMesh->mNumVertices];
		GLuint indices[idxCount]; // assumes triangles

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

			aiColor3D color;
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			vertex.color.r = color.r;
			vertex.color.g = color.g;
			vertex.color.b = color.b;

			vertices[i] = vertex;
		}

		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (size_t ii = 0; ii < face.mNumIndices; ii++) {
				indices[i * 3 + ii] = face.mIndices[ii];
			}
		}

		Mesh *mesh = new Mesh;
		mesh->setVertexCount((GLsizei) (aiMesh->mNumVertices * numComponents * componentSize));
		mesh->setIndexCount(idxCount);
		buffer->initializeMeshBuffers(*mesh, aiMesh->mNumVertices);

		size_t index = 0;
		for (auto vertex : vertices) {
			mesh->vBuffer[index++] = vertex.position.x;
			mesh->vBuffer[index++] = vertex.position.y;
			mesh->vBuffer[index++] = vertex.position.z;
			mesh->vBuffer[index++] = vertex.normal.x;
			mesh->vBuffer[index++] = vertex.normal.y;
			mesh->vBuffer[index++] = vertex.normal.z;
			mesh->vBuffer[index++] = vertex.color.r;
			mesh->vBuffer[index++] = vertex.color.g;
			mesh->vBuffer[index++] = vertex.color.b;
		}

		arrShift(indices, mesh->idxOffset, idxCount);
		memcpy(mesh->iBuffer, indices, sizeof(indices));
		buffer->uploadMesh(*mesh);

		return mesh;
	}

	void processNode(aiNode *node, const aiScene *scene, BufferController *buffer, Mesh **meshes, size_t *meshIndex) {
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
			meshes[*meshIndex] = processMesh(aiMesh, buffer, scene->mMaterials[aiMesh->mMaterialIndex]);
			*meshIndex += 1;
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
		// One reason a model will fail to load is either the obj file isn't found, or an associated material file isn't present.
		std::cerr << "Failed to load model '" << path << "'\n";
		return Model(nullptr, 0);
	}

	size_t meshCount = 0;
	calcMeshCount(&meshCount, scene->mRootNode);

	size_t meshIndex = 0;
	Mesh **meshes = new Mesh*[meshCount];
	processNode(scene->mRootNode, scene, buffer, meshes, &meshIndex);

	return Model(meshes, meshCount);
}

Model::~Model() {
	for (size_t i = 0; i < _numChildren; i++) {
		delete _children[i];
	}
	delete[] _children;
}

Model::Model(Mesh **children, size_t numChildren) : _children(children), _numChildren(numChildren) {
}

void Model::render() {
	for (size_t i = 0; i < _numChildren; i++) {
		_children[i]->render();
	}
}

#pragma clang diagnostic pop
