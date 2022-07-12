#ifndef UINTA_RESOURCE_RESOURCE_MANAGER_HPP
#define UINTA_RESOURCE_RESOURCE_MANAGER_HPP

#include <string>
#include <vector>

#include "uinta/cfg.hpp"
#include "uinta/logging.hpp"
#include "uinta/resource/fwd.hpp"

namespace uinta {

class ResourceManager {
 public:
  ResourceManager(const size_t storageSize = UINTA_RES_MANAGER_STORAGE_SIZE);

  ~ResourceManager();

  void init(const std::string resPaths = RES_PATHS, const char delim = RES_PATHS_DELIM);

  void release(const resource_t* const handle, bool force = false);

  const bool isActive(const resource_t* const handle) const;
  const bool isBuffered(const resource_t* const handle) const;

  const void* getData(const resource_t* const handle) const;
  const char* getDataChars(const resource_t* const handle) const;
  const std::string& getPath(const resource_t* const handle) const;
  const resource_size_t getSize(const resource_t* const handle) const;
  const ResourceType getType(const resource_t* const handle) const;

 protected:
  virtual const logger_t& getLogger() = 0;

  const resource_t* const registerResource(const std::string& relativePath, const ResourceType type);

  void loadAll();
  void loadHandle(const resource_t* const handle);

 private:
  struct ResourceBlock {
    resource_size_t size;
    void* ptr;
    ResourceBlock *back, *forward;

    ResourceBlock(void* ptr = nullptr, resource_size_t size = 0);

    ResourceBlock(const ResourceBlock& other);

    ResourceBlock& operator=(const ResourceBlock& other);

    bool operator==(const ResourceBlock& rhs) const;
  };

  std::vector<std::string> resourcePaths;

  std::vector<resource_t*> handles;
  std::vector<ResourceBlock> blockList;
  std::vector<std::string> handlePaths;

  size_t storageSize;
  void* storage = nullptr;

  const resource_t getId(const resource_t* const handle) const;

  void reserveSpace(const resource_t* const handle);

  void loadHandleData(const resource_t* const handle);
  void loadResourceBinary(const resource_t* const handle);
  void loadResourceText(const resource_t* const handle);

  void setIsActive(const resource_t* const handle, const bool isActive);
  void setIsBuffered(const resource_t* const handle, const bool isLoaded);
  void setPath(const resource_t* const handle, const std::string& path);
  void setType(const resource_t* const handle, const ResourceType type);
};

}  // namespace uinta

#endif  // UINTA_RESOURCE_RESOURCE_MANAGER_HPP
