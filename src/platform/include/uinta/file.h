#ifndef SRC_PLATFORM_INCLUDE_UINTA_FILE_H_
#define SRC_PLATFORM_INCLUDE_UINTA_FILE_H_

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

class FileSystem {
  friend class File;

 public:
  static std::vector<std::string> SearchDirs() noexcept;

  virtual Status copy(
      std::filesystem::path src, std::filesystem::path dst,
      std::filesystem::copy_options =
          std::filesystem::copy_options::overwrite_existing) noexcept = 0;
  virtual bool exists(std::filesystem::path) const noexcept = 0;
  virtual bool isDirectory(std::filesystem::path) const noexcept = 0;

 protected:
  virtual ~FileSystem() noexcept = default;
  virtual void close() noexcept = 0;
  virtual Status open(std::filesystem::path, std::ios::openmode) noexcept = 0;
  virtual bool isOpen() const noexcept = 0;
  virtual std::optional<std::string> readLine() noexcept = 0;
  virtual std::vector<std::string> readAllLines() noexcept = 0;
  virtual std::string readAll() noexcept = 0;
  virtual std::vector<u8> readBinary() noexcept = 0;
};

// FIXME: This needs synchronization handling (mutex)
class FileSystemImpl : public FileSystem {
 public:
  static FileSystemImpl* Instance() noexcept {
    static FileSystemImpl instance;
    return &instance;
  }

  ~FileSystemImpl() noexcept override;
  FileSystemImpl(const FileSystemImpl&) noexcept = delete;
  FileSystemImpl& operator=(const FileSystemImpl&) noexcept = delete;
  FileSystemImpl(FileSystemImpl&&) noexcept = delete;
  FileSystemImpl& operator=(FileSystemImpl&&) noexcept = delete;

  Status copy(
      std::filesystem::path src, std::filesystem::path dst,
      std::filesystem::copy_options =
          std::filesystem::copy_options::overwrite_existing) noexcept override;
  bool exists(std::filesystem::path) const noexcept override;
  bool isDirectory(std::filesystem::path) const noexcept override;

 private:
  std::ifstream ifstream_;

  FileSystemImpl() noexcept = default;

  void close() noexcept override;
  Status open(std::filesystem::path, std::ios::openmode) noexcept override;
  bool isOpen() const noexcept override;
  std::optional<std::string> readLine() noexcept override;
  std::vector<std::string> readAllLines() noexcept override;
  std::string readAll() noexcept override;
  std::vector<u8> readBinary() noexcept override;
};

class File {
 public:
  static std::optional<std::filesystem::path> TryFindPath(
      std::filesystem::path path,
      const FileSystem* fileSystem = FileSystemImpl::Instance()) noexcept;

  File(std::filesystem::path path,
       FileSystem* fileSystem = FileSystemImpl::Instance(),
       std::ios::openmode mode = std::ios::in) noexcept;

  ~File() noexcept;

  File(const File&) noexcept = delete;
  File& operator=(const File&) noexcept = delete;
  File(File&&) noexcept = delete;
  File& operator=(File&&) noexcept = delete;

  std::optional<std::string> readLine() noexcept;

  std::vector<std::string> readAllLines() noexcept;

  std::string readAll() noexcept;

  std::vector<u8> readBinary() noexcept;

  std::filesystem::path path() const noexcept { return path_; }

  const Status& status() const noexcept { return status_; }

 private:
  std::filesystem::path path_;
  std::ios::openmode mode_;
  FileSystem* fileSystem_;
  Status status_;

  Status tryOpen() noexcept;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_FILE_H_
