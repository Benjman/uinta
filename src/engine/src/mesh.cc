#include "uinta/mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "uinta/file.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

std::vector<Primitive> processNode(aiNode* node, const aiScene* scene) noexcept;
Primitive processMesh(aiMesh* mesh, const aiScene*) noexcept;

StatusOr<std::vector<Primitive>> fbx(const std::string_view path) noexcept {
  auto file = File(path.data());
  if (!file.status().ok()) return file.status();
  Assimp::Importer importer;
  const auto* scene = importer.ReadFile(file.path().data(),
                                        aiProcess_Triangulate |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_SortByPType);
  if (!scene)
    return InternalError(StrFormat("Failed to parse %s: %s", file.path().data(),
                                   importer.GetErrorString()));
  return processNode(scene->mRootNode, scene);
}

Primitive processMesh(aiMesh* mesh, const aiScene*) noexcept {
  size_t idxCount = 0;
  std::for_each(&mesh->mFaces[0], &mesh->mFaces[mesh->mNumFaces],
                [&idxCount](auto& v) { idxCount += v.mNumIndices; });
  std::vector<Vertex> vertices;
  std::vector<u32> indices;
  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    auto v = mesh->mVertices[i];
    auto n = mesh->mNormals[i];
    vertices.push_back(Vertex({v.x, v.y, v.z}, {n.x, n.y, n.z}));
  }
  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    auto face = mesh->mFaces[i];
    indices.reserve(face.mNumIndices);
    for (size_t ii = 0; ii < face.mNumIndices; ii++)
      indices.push_back(face.mIndices[ii]);
  }
  return {vertices, indices};
}

std::vector<Primitive> processNode(aiNode* node,
                                   const aiScene* scene) noexcept {
  std::vector<Primitive> result;
  for (u32 i = 0; i < node->mNumMeshes; i++)
    result.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

  for (u32 i = 0; i < node->mNumChildren; i++) {
    auto children = processNode(node->mChildren[i], scene);
    result.insert(result.end(), children.begin(), children.end());
  }

  return result;
}

void assimp() noexcept { auto fs = FileSystemImpl(); }

}  // namespace uinta
