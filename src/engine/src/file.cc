#include "uinta/file.h"

#include <filesystem>
#include <string>

#include "uinta/cfg.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/lib/absl/synchronization.h"

namespace uinta {

File::File(std::string path, std::shared_ptr<FileSystem> fileSystem,
           std::ios::openmode mode) noexcept
    : path_(path), mode_(mode), fileSystem_(std::move(fileSystem)) {
  if (fileSystem_->isOpen()) fileSystem_->close();
  auto tryFindStatus = TryFindPath(path, fileSystem_.get());
  if (!tryFindStatus.ok()) {
    status_ = tryFindStatus.status();
    return;
  }
  path_ = tryFindStatus.value();
}

File::~File() noexcept { path_ = ""; }

File::File(File&& other) noexcept
    : path_(std::move(other.path_)),
      fileSystem_(std::move(other.fileSystem_)) {}

File& File::operator=(File&& other) noexcept {
  if (this != &other) {
    path_ = std::move(other.path_);
    fileSystem_ = std::move(other.fileSystem_);
  }
  return *this;
}

std::optional<std::string> File::readLine() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return {};
  }
  return fileSystem_->readLine();
}

std::vector<std::string> File::readAllLines() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return {};
  }
  return fileSystem_->readAllLines();
}

std::string File::readAll() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return {};
  }
  return fileSystem_->readAll();
}

std::vector<u8> File::readBinary() noexcept {
  if (auto status = tryOpen(); !status.ok()) {
    return {};
  }
  return fileSystem_->readBinary();
}

Status File::tryOpen() noexcept {
  if (!fileSystem_->isOpen()) return fileSystem_->open(path_, mode_);
  return {};
}

StatusOr<std::string> File::TryFindPath(std::string path,
                                        const FileSystem* fileSystem) noexcept {
  assert(fileSystem && "FileSystem* cannot be null.");
  if (path.empty()) return InvalidArgumentError("Path cannot be empty.");
  static once_flag initFlag;
  static std::vector<std::string> searchDirs;
  call_once(initFlag, []() {
    searchDirs = StrSplit(cfg::FileSearchPaths, cfg::FileSearchPathsDelim);
  });
  if (auto status = fileSystem->exists(path); !status.ok()) {
    return status.status();
  }
  for (auto& dir : searchDirs) {
    auto absPath = StrCat(dir, "/", path);
    auto status = fileSystem->exists(absPath);
    if (!status.ok()) return status.status();
    if (status.value()) return absPath;
  }
  return NotFoundError(StrFormat("Failed to locate file: %s", path));
}

FileSystemImpl::~FileSystemImpl() noexcept {
  if (ifstream_.is_open()) ifstream_.close();
}

FileSystemImpl::FileSystemImpl(FileSystemImpl&& other) noexcept
    : ifstream_(std::move(other.ifstream_)) {
  other.ifstream_ = {};
}

FileSystemImpl& FileSystemImpl::operator=(FileSystemImpl&& other) noexcept {
  if (this != &other) {
    if (ifstream_.is_open()) ifstream_.close();
    ifstream_ = std::move(other.ifstream_);

    other.ifstream_ = {};
  }
  return *this;
}

StatusOr<bool> FileSystemImpl::exists(const std::string& path) const noexcept {
  try {
    return std::filesystem::exists(path);
  } catch (const std::filesystem::filesystem_error& e) {
    return InternalError(e.what());
  }
}

void FileSystemImpl::close() noexcept { ifstream_.close(); }

Status FileSystemImpl::open(const std::string& path,
                            std::ios::openmode mode) noexcept {
  try {
    if (ifstream_.is_open()) ifstream_.close();
    ifstream_.open(path, mode);
    return {};
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
  return std::optional<std::string>();
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
