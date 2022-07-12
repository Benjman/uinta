#ifndef UINTA_RESOURCE_MODEL_MANAGER_HPP
#define UINTA_RESOURCE_MODEL_MANAGER_HPP

#include "./resource_manager.hpp"

namespace uinta {

using model_t = resource_t;

const inline char* const MODEL_CUBE_PATH = "model/cube.obj";
const inline char* const MODEL_PLANE_PATH = "model/plane.obj";
const inline char* const MODEL_PYRAMID_PATH = "model/pyramid.obj";
const inline char* const MODEL_SUZANNE_PATH = "model/suzanne.obj";
const inline char* const MODEL_TRI_PATH = "model/tri.obj";

class ModelManager : public ResourceManager {
 public:
  ModelManager(const size_t storageSize = UINTA_RES_MANAGER_STORAGE_SIZE);

  ~ModelManager();

  const resource_t* const registerModel(const std::string& relativePath);

  void loadAllModels();
  void loadModel(const model_t* const handle);

 protected:
  const logger_t& getLogger() override;

 private:
  logger_t logger;
};

}  // namespace uinta

#endif  // UINTA_RESOURCE_MODEL_MANAGER_HPP
