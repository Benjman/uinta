#ifndef UINTA_RESOURCE_FILE_MANAGER_HPP
#define UINTA_RESOURCE_FILE_MANAGER_HPP

#include "./resource_manager.hpp"

namespace uinta {

using file_t = resource_t;

class FileManager : public ResourceManager {
 public:
  FileManager(const size_t storageSize);

  ~FileManager();

  const resource_t* const registerFile(const std::string& relativePath, const ResourceType type);

  void loadAllFiles();

  void loadFile(const file_t* const handle);

 protected:
  const logger_t& getLogger() override;

 private:
  logger_t logger;
};

}  // namespace uinta

#endif  // UINTA_RESOURCE_FILE_MANAGER_HPP
