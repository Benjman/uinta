#ifndef UINTA_MODEL_MANAGER_HPP
#define UINTA_MODEL_MANAGER_HPP

#include <uinta/component/model.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/mesh.hpp>
#include <vector>

namespace uinta {

// TODO ModelManager should take a pointer and size instead of allocating its own memory
class ModelManager {
 public:
  ModelManager() = default;

  uinta_error_code load(model_t& ref, const file_t* const file, const FileManager& fm,
                        const std::unordered_map<MeshAttribType, MeshAttrib>& attribs = {
                            {MeshAttribType::Position, {9, 0}},
                            {MeshAttribType::Normal, {9, 3}},
                            {MeshAttribType::Color, {9, 6}},
                        });

  Model model(const model_t handle) const;
  const file_t* file(const model_t handle) const;
  const f32* const vertexBuffer(const model_t handle) const;
  u32 vertexBufferSize(const model_t handle) const;
  const u32* const indexBuffer(const model_t handle) const;
  u32 indexBufferSize(const model_t handle) const;

 private:
  std::vector<Model> models;
  std::vector<const file_t*> files;
  std::vector<f32*> vertexBuffers;
  std::vector<u32*> indexBuffers;
};

}  // namespace uinta

#endif  // UINTA_MODEL_MANAGER_HPP
