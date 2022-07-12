#include "uinta/resource/model_manager.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace uinta;

ModelManager::ModelManager(const size_t storageSize) : ResourceManager(storageSize) {
  logger = spdlog::stdout_color_mt("ModelManager");
}

ModelManager::~ModelManager() { spdlog::drop("ModelManager"); }

const logger_t& ModelManager::getLogger() { return logger; }

const model_t* const ModelManager::registerModel(const std::string& relativePath) {
  return registerResource(relativePath, ResourceType::Text);
}

void ModelManager::loadAllModels() { loadAll(); }

void ModelManager::loadModel(const model_t* const handle) { loadHandle(handle); }
