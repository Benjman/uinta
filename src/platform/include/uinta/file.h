#ifndef SRC_PLATFORM_INCLUDE_UINTA_FILE_H_
#define SRC_PLATFORM_INCLUDE_UINTA_FILE_H_

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "uinta/cfg.h"
#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

std::array<const char*, cfg::UINTA_FILE_PATHS_COUNT> FileSearchDirs() noexcept;

void AddFileSearchDir(const std::filesystem::path& dir) noexcept;

std::optional<std::filesystem::path> FindFile(const std::filesystem::path& path) noexcept;

Status ReadFile(const std::filesystem::path& path, std::string& out) noexcept;

Status ReadFileBinary(const std::filesystem::path& path, std::vector<u8>& out) noexcept;

Status ReadFileLines(const std::filesystem::path& path, std::vector<std::string>& out) noexcept;

Status CopyFile(const std::filesystem::path& src, const std::filesystem::path& dst,
                std::filesystem::copy_options options = std::filesystem::copy_options::overwrite_existing) noexcept;

class FileReader {
 public:
  static Status Open(const std::filesystem::path& path, FileReader& out) noexcept;

  FileReader() noexcept = default;
  FileReader(FileReader&&) noexcept = default;
  FileReader& operator=(FileReader&&) noexcept = default;
  FileReader(const FileReader&) = delete;
  FileReader& operator=(const FileReader&) = delete;

  Status ReadLine(std::optional<std::string>& out) noexcept;

  Status ReadAllLines(std::vector<std::string>& out) noexcept;

  Status ReadAll(std::string& out) noexcept;

  Status ReadAllBinary(std::vector<u8>& out) noexcept;

  const std::filesystem::path& path() const noexcept { return path_; }
  bool is_open() const noexcept { return stream_.is_open(); }

 private:
  std::ifstream stream_;
  std::filesystem::path path_;

  FileReader(std::ifstream stream, std::filesystem::path path) noexcept;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_FILE_H_
