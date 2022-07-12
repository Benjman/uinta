#include "uinta/resource/file_manager.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace uinta;

FileManager::FileManager(const size_t storageSize) : ResourceManager(storageSize) {
  logger = spdlog::stdout_color_mt("FileManager");
}

FileManager::~FileManager() { spdlog::drop("FileManager"); }

const file_t* const FileManager::registerFile(const std::string& relativePath, const ResourceType type) {
  return registerResource(relativePath, type);
}

void FileManager::loadAllFiles() { loadAll(); }

void FileManager::loadFile(const file_t* const handle) { loadHandle(handle); }

const logger_t& FileManager::getLogger() { return logger; }
