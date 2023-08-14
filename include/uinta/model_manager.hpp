#ifndef UINTA_MODEL_MANAGER_HPP
#define UINTA_MODEL_MANAGER_HPP

#include <uinta/file_manager.hpp>
#include <uinta/mesh.hpp>
#include <vector>

namespace uinta {

using model_t = u16;

struct Model {
  model_t id;  // TODO move to flags
  u32 vertexCount;
  u32 indexCount;
  u32 indexOffset;

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
  const f32* const getVertexBuffer(const model_t handle) const;
  u32 getVertexBufferSize(const model_t handle) const;
  const u32* const getIndexBuffer(const model_t handle) const;
  u32 getIndexBufferSize(const model_t handle) const;

 private:
  std::vector<Model> models;
  std::vector<const file_t*> files;
  std::vector<f32*> vertexBuffers;
  std::vector<u32*> indexBuffers;
};

}  // namespace uinta

#endif  // UINTA_MODEL_MANAGER_HPP
