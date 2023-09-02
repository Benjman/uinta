#include <uinta/gl/api.h>

#include <cstring>
#include <uinta/error.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/parsers/obj.hpp>

namespace uinta {

constexpr u32 HANDLE_ID_MASK = 0xFFFF;

uinta_error_code ModelManager::load(model_t& ref, const file_t* const file, const FileManager& fm,
                                    const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  Model m;
  m.id = ref = models.size();
  const auto fileSize = fm.getSize(file);
  f32 vertices[fileSize];
  u32 indices[fileSize];
  u32 indexOffset = !m.id ? 0 : model(m.id - 1).indexOffset + model(m.id).indexCount;

  if (auto error = loadObj(fm.getDataString(file), vertices, &m.vertexCount, indices, &m.indexCount, &indexOffset, attribs);
      error)
    return error;

  vertexBuffers.push_back(new f32[m.vertexCount]);  // TODO better allocator
  memcpy(vertexBuffers.at(m.id), vertices, m.vertexCount * sizeof(f32));

  indexBuffers.push_back(new u32[m.indexCount]);  // TODO better allocator
  memcpy(indexBuffers.at(m.id), indices, m.indexCount * sizeof(u32));

  files.push_back(file);
  models.push_back(m);

  return SUCCESS_EC;
}

Model ModelManager::model(const model_t handle) const {
  return models.at(handle & HANDLE_ID_MASK);
}

const file_t* ModelManager::file(const model_t handle) const {
  return files.at(handle & HANDLE_ID_MASK);
}

const f32* const ModelManager::vertexBuffer(const model_t handle) const {
  return vertexBuffers.at(handle & HANDLE_ID_MASK);
}

u32 ModelManager::vertexBufferSize(const model_t handle) const {
  return model(handle).vertexCount * sizeof(f32);
}

const u32* const ModelManager::indexBuffer(const model_t handle) const {
  return indexBuffers.at(handle & HANDLE_ID_MASK);
}

u32 ModelManager::indexBufferSize(const model_t handle) const {
  return model(handle).indexCount * sizeof(GLuint);
}

}  // namespace uinta
