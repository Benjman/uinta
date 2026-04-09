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

// Returns the configured search directories for file resolution.
// Thread-safe, initialized once on first call.
std::array<const char*, cfg::UINTA_FILE_PATHS_COUNT> FileSearchDirs() noexcept;

// Attempts to resolve a path by checking if it exists directly, or within
// any of the configured search directories. Returns the resolved absolute
// path if found, std::nullopt otherwise.
std::optional<std::filesystem::path> FindFile(
    const std::filesystem::path& path) noexcept;

// Reads the entire file as text. Returns error status if the file cannot
// be opened or read.
Status ReadFile(const std::filesystem::path& path, std::string& out) noexcept;

// Reads the entire file as binary data. Returns error status if the file
// cannot be opened or read.
Status ReadFileBinary(const std::filesystem::path& path,
                      std::vector<u8>& out) noexcept;

// Reads all lines from a text file. Returns error status if the file cannot
// be opened or read.
Status ReadFileLines(const std::filesystem::path& path,
                     std::vector<std::string>& out) noexcept;

// Copies a file from src to dst. Creates parent directories if needed.
// Returns error status if the operation fails.
Status CopyFile(const std::filesystem::path& src,
                const std::filesystem::path& dst,
                std::filesystem::copy_options options =
                    std::filesystem::copy_options::overwrite_existing) noexcept;

// RAII file reader for line-by-line reading of large files.
// Movable but not copyable. Use the static Open() factory method.
class FileReader {
 public:
  // Opens a file for reading. Returns error status if the file cannot be
  // opened.
  static Status Open(const std::filesystem::path& path,
                     FileReader& out) noexcept;

  FileReader() noexcept = default;
  FileReader(FileReader&&) noexcept = default;
  FileReader& operator=(FileReader&&) noexcept = default;
  FileReader(const FileReader&) = delete;
  FileReader& operator=(const FileReader&) = delete;

  // Reads the next line from the file. Returns std::nullopt on EOF.
  // Returns error status if a read error occurs.
  Status ReadLine(std::optional<std::string>& out) noexcept;

  // Reads all remaining lines from the current position.
  Status ReadAllLines(std::vector<std::string>& out) noexcept;

  // Reads all remaining content from the current position as text.
  Status ReadAll(std::string& out) noexcept;

  // Reads all remaining content from the current position as binary.
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
