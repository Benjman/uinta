#ifndef UINTA_IO_FILE_MANAGER_HPP
#define UINTA_IO_FILE_MANAGER_HPP

#include <string>
#include <vector>

#include "uinta/cfg.hpp"
#include "uinta/io/fwd.hpp"
#include "uinta/logging.hpp"
#include "uinta/memory/memory_link.hpp"

namespace uinta {

class FileManager {
 public:
  FileManager(const size_t storageSize = UINTA_FILE_MANAGER_STORAGE_SIZE);

  ~FileManager();

  void init(const std::string searchPaths = UINTA_FILE_SEARCH_PATHS, const char delim = UINTA_FILE_SEARCH_PATHS_DELIM);

  const file_t* const registerFile(const std::string& relativePath, const FileType type);

  void releaseFile(const file_t* const handle, bool force = false);

  const bool isActive(const file_t* const handle) const;
  const bool isBuffered(const file_t* const handle) const;

  const void* getData(const file_t* const handle) const;
  const char* getDataChars(const file_t* const handle) const;
  const std::string& getPath(const file_t* const handle) const;
  const file_size_t getSize(const file_t* const handle) const;
  const FileType getType(const file_t* const handle) const;

  void loadAll();
  void loadHandle(const file_t* const handle);

 private:
  std::vector<std::string> fileSearchPaths;

  std::vector<file_t*> handles;
  std::vector<MemoryLink> blockList;
  std::vector<std::string> handlePaths;

  size_t storageSize;
  void* storage = nullptr;

  const file_t getId(const file_t* const handle) const;

  void reserveSpace(const file_t* const handle);

  void loadHandleData(const file_t* const handle);

  void loadFileBinary(const file_t* const handle);
  void loadFileText(const file_t* const handle);

  void setIsActive(const file_t* const handle, const bool isActive);
  void setIsBuffered(const file_t* const handle, const bool isLoaded);
  void setPath(const file_t* const handle, const std::string& path);
  void setType(const file_t* const handle, const FileType type);
};

}  // namespace uinta

#endif  // UINTA_IO_FILE_MANAGER_HPP
