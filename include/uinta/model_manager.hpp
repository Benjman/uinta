#ifndef UINTA_MODEL_MANAGER_HPP
#define UINTA_MODEL_MANAGER_HPP

#include <string>
#include <uinta/file_manager.hpp>
#include <uinta/mesh.hpp>
#include <vector>

namespace uinta {

struct Model {
  model_t id;  // TODO move to flags
  uint vertexCount;
  uint indexCount;
  uint indexOffset;

  Model() : id(0), vertexCount(0), indexCount(0), indexOffset(0) {
  }

  Model(const Model& other) {
    *this = other;
  }

  Model& operator=(const Model& rhs) {
    id = rhs.id;
    vertexCount = rhs.vertexCount;
    indexCount = rhs.indexCount;
    indexOffset = rhs.indexOffset;
    return *this;
  }
};

// TODO ModelManager should take a pointer and size instead of allocating its own memory
class ModelManager {
 public:
  explicit ModelManager() {
  }

  ModelManager(const ModelManager& other) {
  }

  model_t loadModel(const file_t* const file, const FileManager* fm,
                    const std::unordered_map<MeshAttribType, MeshAttrib>& attribs = {
                        {MeshAttribType_Position, {9, 0}},
                        {MeshAttribType_Normal, {9, 3}},
                        {MeshAttribType_Color, {9, 6}},
                    });

  Model getModel(const model_t handle) const;
  const file_t* getFile(const model_t handle) const;
  const float* const getVertexBuffer(const model_t handle) const;
  uint getVertexBufferSize(const model_t handle) const;
  const uint* const getIndexBuffer(const model_t handle) const;
  uint getIndexBufferSize(const model_t handle) const;

 private:
  std::vector<Model> models;
  std::vector<const file_t*> files;
  std::vector<float*> vertexBuffers;
  std::vector<uint*> indexBuffers;
};

}  // namespace uinta

#endif  // UINTA_MODEL_MANAGER_HPP
