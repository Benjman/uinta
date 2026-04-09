#include "uinta/file.h"

#include <absl/base/call_once.h>
#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "uinta/status.h"

namespace uinta {

std::array<const char*, cfg::UINTA_FILE_PATHS_COUNT> FileSearchDirs() noexcept {
  static absl::once_flag initFlag;
  call_once(initFlag, []() {
    for (const auto& path : cfg::UINTA_FILE_PATHS) {
      LOG(INFO) << absl::StrFormat("Search path registered: %s", path);
    }
  });
  return cfg::UINTA_FILE_PATHS;
}

std::optional<std::filesystem::path> FindFile(
    const std::filesystem::path& path) noexcept {
  if (path.empty()) {
    return std::nullopt;
  }

  std::error_code ec;
  if (std::filesystem::exists(path, ec)) {
    return std::filesystem::absolute(path, ec);
  }

  for (const auto& dir : FileSearchDirs()) {
    auto candidate = std::filesystem::path(dir) / path;
    if (std::filesystem::exists(candidate, ec)) {
      return std::filesystem::absolute(candidate, ec);
    }
  }

  return std::nullopt;
}

Status ReadFile(const std::filesystem::path& path, std::string& out) noexcept {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    return NotFoundError(
        absl::StrFormat("Failed to open file: %s", path.string()));
  }

  file.seekg(0, std::ios::end);
  auto pos = file.tellg();
  if (pos == -1) {
    return InternalError(
        absl::StrFormat("Failed to get file size: %s", path.string()));
  }

  auto size = static_cast<size_t>(pos);
  file.seekg(0, std::ios::beg);

  out.resize(size);
  if (!file.read(out.data(), static_cast<std::streamsize>(size))) {
    return InternalError(
        absl::StrFormat("Failed to read file: %s", path.string()));
  }

  return OkStatus();
}

Status ReadFileBinary(const std::filesystem::path& path,
                      std::vector<u8>& out) noexcept {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    return NotFoundError(
        absl::StrFormat("Failed to open file: %s", path.string()));
  }

  file.seekg(0, std::ios::end);
  auto pos = file.tellg();
  if (pos == -1) {
    return InternalError(
        absl::StrFormat("Failed to get file size: %s", path.string()));
  }

  auto size = static_cast<size_t>(pos);
  file.seekg(0, std::ios::beg);

  out.resize(size);
  if (!file.read(reinterpret_cast<char*>(out.data()),
                 static_cast<std::streamsize>(size))) {
    return InternalError(
        absl::StrFormat("Failed to read file: %s", path.string()));
  }

  return OkStatus();
}

Status ReadFileLines(const std::filesystem::path& path,
                     std::vector<std::string>& out) noexcept {
  std::ifstream file(path);
  if (!file) {
    return NotFoundError(
        absl::StrFormat("Failed to open file: %s", path.string()));
  }

  out.clear();
  std::string line;
  while (std::getline(file, line)) {
    out.push_back(std::move(line));
  }

  if (file.bad()) {
    return InternalError(
        absl::StrFormat("Error reading file: %s", path.string()));
  }

  return OkStatus();
}

Status CopyFile(const std::filesystem::path& src,
                const std::filesystem::path& dst,
                std::filesystem::copy_options options) noexcept {
  std::error_code ec;

  if (auto parent = dst.parent_path(); !parent.empty()) {
    if (!std::filesystem::exists(parent, ec)) {
      std::filesystem::create_directories(parent, ec);
      if (ec) {
        return InternalError(
            absl::StrFormat("Failed to create directory %s: %s",
                            parent.string(), ec.message()));
      }
    }
  }

  std::filesystem::copy(src, dst, options, ec);
  if (ec) {
    return InternalError(absl::StrFormat("Failed to copy %s to %s: %s",
                                         src.string(), dst.string(),
                                         ec.message()));
  }

  return OkStatus();
}

FileReader::FileReader(std::ifstream stream,
                       std::filesystem::path path) noexcept
    : stream_(std::move(stream)), path_(std::move(path)) {}

Status FileReader::Open(const std::filesystem::path& path,
                        FileReader& out) noexcept {
  std::ifstream stream(path, std::ios::in | std::ios::binary);
  if (!stream) {
    return NotFoundError(
        absl::StrFormat("Failed to open file: %s", path.string()));
  }

  out = FileReader(std::move(stream), path);
  return OkStatus();
}

Status FileReader::ReadLine(std::optional<std::string>& out) noexcept {
  std::string line;
  if (std::getline(stream_, line)) {
    out = std::move(line);
    return OkStatus();
  }

  if (stream_.eof()) {
    out = std::nullopt;
    return OkStatus();
  }

  return InternalError(
      absl::StrFormat("Error reading line from: %s", path_.string()));
}

Status FileReader::ReadAllLines(std::vector<std::string>& out) noexcept {
  out.clear();
  std::string line;
  while (std::getline(stream_, line)) {
    out.push_back(std::move(line));
  }

  if (stream_.bad()) {
    return InternalError(
        absl::StrFormat("Error reading file: %s", path_.string()));
  }

  return OkStatus();
}

Status FileReader::ReadAll(std::string& out) noexcept {
  auto start = stream_.tellg();
  stream_.seekg(0, std::ios::end);
  auto end = stream_.tellg();

  if (start == -1 || end == -1) {
    return InternalError(
        absl::StrFormat("Failed to get file size: %s", path_.string()));
  }

  auto size = static_cast<size_t>(end - start);
  stream_.seekg(start, std::ios::beg);

  out.resize(size);
  if (!stream_.read(out.data(), static_cast<std::streamsize>(size))) {
    return InternalError(
        absl::StrFormat("Failed to read file: %s", path_.string()));
  }

  return OkStatus();
}

Status FileReader::ReadAllBinary(std::vector<u8>& out) noexcept {
  auto start = stream_.tellg();
  stream_.seekg(0, std::ios::end);
  auto end = stream_.tellg();

  if (start == -1 || end == -1) {
    return InternalError(
        absl::StrFormat("Failed to get file size: %s", path_.string()));
  }

  auto size = static_cast<size_t>(end - start);
  stream_.seekg(start, std::ios::beg);

  out.resize(size);
  if (!stream_.read(reinterpret_cast<char*>(out.data()),
                    static_cast<std::streamsize>(size))) {
    return InternalError(
        absl::StrFormat("Failed to read file: %s", path_.string()));
  }

  return OkStatus();
}

}  // namespace uinta
