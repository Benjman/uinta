#ifndef SRC_ENGINE_INCLUDE_UINTA_FILE_H_
#define SRC_ENGINE_INCLUDE_UINTA_FILE_H_

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "uinta/lib/absl/status.h"
#include "uinta/types.h"

namespace uinta {

class FileSystem {
  friend class File;

 protected:
  virtual ~FileSystem() noexcept = default;
  virtual StatusOr<bool> exists(const std::string&) const noexcept = 0;
  virtual void close() noexcept = 0;
  virtual Status open(const std::string&, std::ios::openmode) noexcept = 0;
  virtual bool isOpen() const noexcept = 0;
  virtual std::optional<std::string> readLine() noexcept = 0;
  virtual std::vector<std::string> readAllLines() noexcept = 0;
  virtual std::string readAll() noexcept = 0;
  virtual std::vector<u8> readBinary() noexcept = 0;
};

class FileSystemImpl : public FileSystem {
 public:
  FileSystemImpl() noexcept = default;
  ~FileSystemImpl() noexcept override;

  FileSystemImpl(const FileSystemImpl&) noexcept = delete;
  FileSystemImpl& operator=(const FileSystemImpl&) noexcept = delete;
  FileSystemImpl(FileSystemImpl&&) noexcept = delete;
  FileSystemImpl& operator=(FileSystemImpl&&) noexcept = delete;

 private:
  std::ifstream ifstream_;

  StatusOr<bool> exists(const std::string&) const noexcept override;
  void close() noexcept override;
  Status open(const std::string&, std::ios::openmode) noexcept override;
  bool isOpen() const noexcept override;
  std::optional<std::string> readLine() noexcept override;
  std::vector<std::string> readAllLines() noexcept override;
  std::string readAll() noexcept override;
  std::vector<u8> readBinary() noexcept override;
};

class File {
 public:
  static StatusOr<std::string> TryFindPath(
      std::string path, const FileSystem* fileSystem) noexcept;

  File(std::string path,
       std::shared_ptr<FileSystem> fileSystem =
           std::make_shared<FileSystemImpl>(),
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

  const std::string_view path() const noexcept { return path_; }

  const Status& status() const noexcept { return status_; }

 private:
  std::string path_;
  std::ios::openmode mode_;
  std::shared_ptr<FileSystem> fileSystem_;
  Status status_;

  Status tryOpen() noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_FILE_H_
