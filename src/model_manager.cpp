#include <uinta/gl/api.h>

#include <cstring>
#include <uinta/error.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/parsers/obj.hpp>

namespace uinta {

constexpr u32 HANDLE_ID_MASK = 0xFFFF;

uinta_error_code ModelManager::loadModel(model_t& ref, const file_t* const file, const FileManager& fm,
                                         const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  Model model;
  model.id = ref = models.size();
  const auto fileSize = fm.getSize(file);
  f32 vertices[fileSize];
  u32 indices[fileSize];
  u32 indexOffset = !model.id ? 0 : getModel(model.id - 1).indexOffset + getModel(model.id).indexCount;

  if (auto error =
          loadObj(fm.getDataString(file), vertices, &model.vertexCount, indices, &model.indexCount, &indexOffset, attribs);
      error)
    return error;

  vertexBuffers.push_back(new f32[model.vertexCount]);  // TODO better allocator
  memcpy(vertexBuffers.at(model.id), vertices, model.vertexCount * sizeof(f32));

  indexBuffers.push_back(new u32[model.indexCount]);  // TODO better allocator
  memcpy(indexBuffers.at(model.id), indices, model.indexCount * sizeof(u32));

  files.push_back(file);
  models.push_back(model);

  return SUCCESS_EC;
}

Model ModelManager::getModel(const model_t handle) const {
  return models.at(handle & HANDLE_ID_MASK);
}

const file_t* ModelManager::getFile(const model_t handle) const {
  return files.at(handle & HANDLE_ID_MASK);
}

const f32* const ModelManager::getVertexBuffer(const model_t handle) const {
  return vertexBuffers.at(handle & HANDLE_ID_MASK);
}

u32 ModelManager::getVertexBufferSize(const model_t handle) const {
  return getModel(handle).vertexCount * sizeof(f32);
}

const u32* const ModelManager::getIndexBuffer(const model_t handle) const {
  return indexBuffers.at(handle & HANDLE_ID_MASK);
}

u32 ModelManager::getIndexBufferSize(const model_t handle) const {
  return getModel(handle).indexCount * sizeof(GLuint);
}

}  // namespace uinta
