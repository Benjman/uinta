#include <cstring>
#include <uinta/gl/vao.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/parsers/obj.hpp>

namespace uinta {

const uint HANDLE_ID_MASK = 0xFFFF;

model_t ModelManager::loadModel(const file_t* const file, const FileManager* fm,
                                const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  Model model;
  model.id = models.size();
  auto fileSize = fm->getSize(file);
  float vertices[fileSize];
  uint indices[fileSize];
  uint indexOffset = !model.id ? 0 : getModel(model.id - 1).indexOffset + getModel(model.id).indexCount;

  loadObj(fm->getDataString(file), vertices, &model.vertexCount, indices, &model.indexCount, &indexOffset, attribs);

  vertexBuffers.push_back(new float[model.vertexCount]);  // TODO better allocator
  memcpy(vertexBuffers.at(model.id), vertices, model.vertexCount * sizeof(float));

  indexBuffers.push_back(new uint[model.indexCount]);  // TODO better allocator
  memcpy(indexBuffers.at(model.id), indices, model.indexCount * sizeof(uint));

  files.push_back(file);
  models.push_back(model);
  return model.id;
}

Model ModelManager::getModel(const model_t handle) const {
  return models.at(handle & HANDLE_ID_MASK);
}

const file_t* ModelManager::getFile(const model_t handle) const {
  return files.at(handle & HANDLE_ID_MASK);
}

const float* const ModelManager::getVertexBuffer(const model_t handle) const {
  return vertexBuffers.at(handle & HANDLE_ID_MASK);
}

uint ModelManager::getVertexBufferSize(const model_t handle) const {
  return getModel(handle).vertexCount * sizeof(float);
}

const uint* const ModelManager::getIndexBuffer(const model_t handle) const {
  return indexBuffers.at(handle & HANDLE_ID_MASK);
}

uint ModelManager::getIndexBufferSize(const model_t handle) const {
  return getModel(handle).indexCount * sizeof(GLuint);
}

}  // namespace uinta
