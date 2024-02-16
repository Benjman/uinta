#include "uinta/file.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "absl/base/call_once.h"
#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "uinta/cfg.h"
#include "uinta/status.h"

namespace uinta {

std::vector<std::string> FileSystem::SearchDirs() noexcept {
  static std::vector<std::string> searchDirs;
  static absl::once_flag initFlag;
  call_once(initFlag, []() {
    searchDirs = absl::StrSplit(cfg::FileSearchPaths, ';');
    std::for_each(searchDirs.begin(), searchDirs.end(), [](const auto& path) {
      LOG(INFO) << absl::StrFormat("Search path registered: %s", path);
    });
  });
  return searchDirs;
}

File::File(std::filesystem::path path, FileSystem* fileSystem,
           std::ios::openmode mode) noexcept
    : path_(path), mode_(mode), fileSystem_(fileSystem) {
  if (fileSystem_->isOpen()) fileSystem_->close();
  if (auto opt = TryFindPath(path, fileSystem_); opt.has_value()) {
    path_ = opt.value();
  } else {
    status_ =
        NotFoundError(absl::StrFormat("File not found: '%s'", path.string()));
    return;
  }
}

File::~File() noexcept { path_ = ""; }

std::optional<std::string> File::readLine() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return std::nullopt;
  }
  return fileSystem_->readLine();
}

std::vector<std::string> File::readAllLines() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return std::vector<std::string>();
  }
  return fileSystem_->readAllLines();
}

std::string File::readAll() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return std::string();
  }
  return fileSystem_->readAll();
}

std::vector<u8> File::readBinary() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return std::vector<u8>();
  }
  return fileSystem_->readBinary();
}

Status File::tryOpen() noexcept {
  if (!fileSystem_->isOpen()) return fileSystem_->open(path_, mode_);
  return OkStatus();
}

std::optional<std::filesystem::path> File::TryFindPath(
    std::filesystem::path path, const FileSystem* fileSystem) noexcept {
  assert(fileSystem && "FileSystem* cannot be null.");
  if (path.empty()) return std::nullopt;
  if (fileSystem->exists(path)) {
    return path;
  }
  for (auto& dir : FileSystem::SearchDirs()) {
    auto absPath = absl::StrCat(dir, "/", path.string());
    if (fileSystem->exists(absPath)) {
      return absPath;
    }
  }
  return std::nullopt;
}

FileSystemImpl::~FileSystemImpl() noexcept {
  if (ifstream_.is_open()) ifstream_.close();
}

bool FileSystemImpl::exists(std::filesystem::path path) const noexcept {
  try {
    return std::filesystem::exists(path);
  } catch (const std::filesystem::filesystem_error& e) {
    LOG(FATAL) << e.what();
  }
}

bool FileSystemImpl::isDirectory(std::filesystem::path path) const noexcept {
  try {
    return std::filesystem::is_directory(path);
  } catch (const std::filesystem::filesystem_error& e) {
    LOG(FATAL) << e.what();
  }
}

void FileSystemImpl::close() noexcept { ifstream_.close(); }

Status FileSystemImpl::copy(std::filesystem::path src,
                            std::filesystem::path dst,
                            std::filesystem::copy_options options) noexcept {
  if (!std::filesystem::exists(dst.parent_path())) {
    std::filesystem::create_directories(dst.parent_path());
  }
  try {
    std::filesystem::copy(src, dst, options);
    return OkStatus();
  } catch (const std::exception& e) {
    return InternalError(e.what());
  }
}

Status FileSystemImpl::open(std::filesystem::path path,
                            std::ios::openmode mode) noexcept {
  try {
    if (ifstream_.is_open()) ifstream_.close();
    ifstream_.open(path, mode);
    return OkStatus();
  } catch (const std::ios_base::failure& e) {
    return InternalError(e.what());
  }
}

bool FileSystemImpl::isOpen() const noexcept { return ifstream_.is_open(); }

std::optional<std::string> FileSystemImpl::readLine() noexcept {
  std::string line;
  if (std::getline(ifstream_, line)) {
    return line;
  }
  return std::string();
}

std::vector<std::string> FileSystemImpl::readAllLines() noexcept {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(ifstream_, line)) lines.push_back(line);
  return lines;
}

std::string FileSystemImpl::readAll() noexcept {
  ifstream_.seekg(0, std::ios::end);
  size_t size = ifstream_.tellg();
  ifstream_.seekg(0);
  std::string buffer(size, ' ');
  ifstream_.read(&buffer[0], size);
  return buffer;
}

std::vector<u8> FileSystemImpl::readBinary() noexcept {
  ifstream_.seekg(0, std::ios::end);
  size_t size = ifstream_.tellg();
  ifstream_.seekg(0);
  std::vector<u8> buffer(size);
  ifstream_.read(reinterpret_cast<char*>(buffer.data()), size);
  return buffer;
}

}  // namespace uinta
