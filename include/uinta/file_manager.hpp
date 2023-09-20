#ifndef UINTA_IO_FILE_MANAGER_HPP
#define UINTA_IO_FILE_MANAGER_HPP

#include <spdlog/fwd.h>
#include <uinta/flags.h>
#include <uinta/types.h>
#include <uinta/utils/macros.h>

#include <memory>
#include <string>
#include <uinta/cfg.hpp>
#include <uinta/fwd.hpp>
#include <vector>

namespace uinta {

using file_size_t = u32;
using file_t = u32;

// TODO file_t, and associated pointers, should really be smart pointers

class FileManager {
 public:
  explicit FileManager(const size_t storageSize = MEGABYTES(10));

  ~FileManager();

  uinta_error_code init(const Runner& runner, const std::string& searchPaths = UINTA_FILE_SEARCH_PATHS,
                        const char delim = UINTA_FILE_SEARCH_PATHS_DELIM);

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

 protected:
  struct MemoryLink {  // TODO: Get rid of this monstrosity
    size_t size;
    void* ptr;
    MemoryLink *back, *forward;

    explicit MemoryLink(void* ptr = nullptr, size_t size = 0);

    MemoryLink(const MemoryLink& other);

    MemoryLink& operator=(const MemoryLink& other);

    bool operator==(const MemoryLink& rhs) const;
  };

  virtual void loadFileBinary(const file_t* const handle) = 0;
  virtual void loadFileText(const file_t* const handle) = 0;

  const spdlog::logger* logger() const noexcept {
    return m_logger.get();
  }

  std::vector<MemoryLink>& links() noexcept {
    return m_links;
  }

  const file_t getId(const file_t* const handle) const;

 private:
  std::vector<std::string> m_searchPaths;

  std::vector<file_t*> m_handles;
  std::vector<MemoryLink> m_links;
  std::vector<std::string> m_handlePaths;

  std::shared_ptr<spdlog::logger> m_logger;

  size_t m_storageSize;
  void* m_storage = nullptr;

  flags_t m_flags = 0;

  void reserveSpace(const file_t* const handle);
  void parseFileSearchPaths(const std::string& searchPaths, const char delim);
  std::string findPath(const std::string& path);
  void loadHandleData(const file_t* const handle);

  void setIsActive(const file_t* const handle, const bool isActive);
  void setIsBuffered(const file_t* const handle, const bool isLoaded);
  void setPath(const file_t* const handle, const std::string& path);

  bool isInitialized() const;
};

class FileManager_Desktop final : public FileManager {
 protected:
  void loadFileBinary(const file_t* const handle) override;
  void loadFileText(const file_t* const handle) override;
};

}  // namespace uinta

#endif  // UINTA_IO_FILE_MANAGER_HPP
