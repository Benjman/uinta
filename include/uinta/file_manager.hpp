#ifndef UINTA_IO_FILE_MANAGER_HPP
#define UINTA_IO_FILE_MANAGER_HPP

#include <uinta/utils/flags.h>
#include <uinta/utils/macros.h>

#include <string>
#include <uinta/cfg.hpp>
#include <uinta/memory/memory_link.hpp>
#include <vector>

namespace uinta {

// TODO file_t should really be std::shared_ptr<uint>
// TODO all the file_t* should instead be a std::shared_ptr<uint>

using file_t = uint;
using file_size_t = uint;

class FileManager {
 public:
  explicit FileManager(const size_t storageSize = MEGABYTES(10));

  ~FileManager();

  bool init(const std::string& searchPaths = UINTA_FILE_SEARCH_PATHS, const char delim = UINTA_FILE_SEARCH_PATHS_DELIM);

  const file_t* const registerFile(const std::string& relativePath);

  const bool isActive(const file_t* const handle) const;
  const bool isBuffered(const file_t* const handle) const;

  const void* getData(const file_t* const handle) const;
  std::string getDataString(const file_t* const handle) const;
  std::string getPath(const file_t* const handle) const;
  const file_size_t getSize(const file_t* const handle) const;

  void loadAll();
  void loadFile(const file_t* const handle);
  void loadFile(const std::vector<const file_t*>& handles);
  void releaseFile(const file_t* const handle, bool force = false);
  void releaseFile(const std::vector<const file_t*>& handles);

 private:
  std::vector<std::string> fileSearchPaths;

  std::vector<file_t*> handles;
  std::vector<MemoryLink> links;
  std::vector<std::string> handlePaths;

  size_t storageSize;
  void* storage = nullptr;

  flags_t flags = 0;

  void reserveSpace(const file_t* const handle);
  void parseFileSearchPaths(const std::string& searchPaths, const char delim);
  std::string findPath(const std::string& path);

  void loadHandleData(const file_t* const handle);
  void loadFileBinary(const file_t* const handle);
  void loadFileText(const file_t* const handle);

  const file_t getId(const file_t* const handle) const;

  void setIsActive(const file_t* const handle, const bool isActive);
  void setIsBuffered(const file_t* const handle, const bool isLoaded);
  void setPath(const file_t* const handle, const std::string& path);

  bool isInitialized() const;
};

}  // namespace uinta

#endif  // UINTA_IO_FILE_MANAGER_HPP
