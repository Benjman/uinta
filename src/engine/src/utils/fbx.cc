#include "uinta/utils/fbx.h"

#include <iterator>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "uinta/file.h"

namespace uinta {

std::vector<Mesh> processNode(aiNode* node, const aiScene* scene) noexcept;
Mesh processMesh(aiMesh* mesh, const aiScene*) noexcept;

StatusOr<std::vector<Mesh>> loadFbx(const std::string_view path) noexcept {
  auto file = File(path.data());
  if (!file.status().ok()) return file.status();
  Assimp::Importer importer;
  const auto* scene = importer.ReadFile(
      file.path(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                       aiProcess_SortByPType);
  if (!scene)
    return InternalError(absl::StrFormat("Failed to parse %s: %s", file.path(),
                                         importer.GetErrorString()));
  auto result = processNode(scene->mRootNode, scene);
  LOG(INFO) << "Loaded .fbx: " << file.path();
  return result;
}

Mesh processMesh(aiMesh* mesh, const aiScene*) noexcept {
  size_t idxCount = 0;
  std::for_each(&mesh->mFaces[0], &mesh->mFaces[mesh->mNumFaces],
                [&idxCount](auto& v) { idxCount += v.mNumIndices; });
  std::vector<Vertex> vertices;
  std::vector<idx_t> indices;
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

std::vector<Mesh> processNode(aiNode* node, const aiScene* scene) noexcept {
  std::vector<Mesh> result;
  for (u32 i = 0; i < node->mNumMeshes; i++)
    result.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

  for (u32 i = 0; i < node->mNumChildren; i++) {
    auto children = processNode(node->mChildren[i], scene);
    result.insert(result.end(), std::make_move_iterator(children.begin()),
                  std::make_move_iterator(children.end()));
  }

  return result;
}

}  // namespace uinta
