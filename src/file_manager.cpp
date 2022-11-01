#include <filesystem>
#include <fstream>
#include <uinta/file_manager.hpp>
#include <uinta/logging.hpp>
#include <uinta/memory/memory_link.hpp>

namespace uinta {

inline const file_t UINTA_FILE_ID_MASK = 0xFFFF;
inline const file_t UINTA_FILE_IS_BUFFERED_MASK = 0x70000;
inline const file_t UINTA_FILE_IS_ACTIVE_MASK = 0x80000;

inline const flag_t FILEMANAGER_INITIALIZED = 1 << 0;

inline std::string sanitizePath(const std::string& searchPath, const std::string& path) {
  // TODO ensure searchPath, or path aren't double slashing "//", or that a slash isn't missing between the two
  return searchPath + path;
}

FileManager::FileManager(const size_t storageSize) : storageSize(storageSize), storage(malloc(storageSize)) {
}

FileManager::~FileManager() {
  for (auto* handle : handles) {
    releaseFile(handle);
    delete handle;
    handle = nullptr;
  }
  free(storage);
  storage = nullptr;
  storageSize = 0;
}

bool FileManager::init(const std::string& searchPaths, const char delim) {
  if (isFlagSet(FILEMANAGER_INITIALIZED, flags)) SPDLOG_WARN("Too many calls to FileManager::init()!");
  parseFileSearchPaths(searchPaths, delim);
  setFlag(FILEMANAGER_INITIALIZED, true, flags);
  loadAll();
  return true;
}

const file_t* const FileManager::registerFile(const std::string& relativePath) {
  if (handles.size() >= UINTA_FILE_ID_MASK) {
    SPDLOG_CRITICAL("Attempt made to exceed maximum handle size of {}", UINTA_FILE_ID_MASK);
    return nullptr;
  }
  auto* handle = handles.emplace_back(new file_t(handles.size()));
  handlePaths.emplace_back(relativePath);
  links.emplace_back(MemoryLink());

  releaseFile(handle, true);
  setIsActive(handle, true);

  SPDLOG_DEBUG("Registered file '{}'.", relativePath);
  return handle;
}

void FileManager::releaseFile(const file_t* const handle, bool force) {
  if (!force && !isActive(handle)) return;
  if (isBuffered(handle)) {
    SPDLOG_DEBUG("Releasing file '{}'.", getPath(handle));
  }
  setPath(handle, "");
  setIsBuffered(handle, false);
  setIsActive(handle, false);
  auto& link = links.at(getId(handle));
  if (link.back) link.back->forward = link.forward;
  if (link.forward) link.forward->back = link.back;
  link = MemoryLink();
}

void FileManager::releaseFile(const std::vector<const file_t*>& handles) {
  for (auto* handle : handles) {
    releaseFile(handle);
  }
}

const bool FileManager::isActive(const file_t* const handle) const {
  if (!isInitialized()) return false;
  if (handle == nullptr || getId(handle) > handles.size()) {
    SPDLOG_WARN("Invalid handle {}!", *handle);
  }
  if (handles.at(getId(handle)) != handle) return false;
  return *handle & UINTA_FILE_IS_ACTIVE_MASK;
}

const bool FileManager::isBuffered(const file_t* const handle) const {
  if (!isActive(handle)) return false;
  if (!(isActive(handle) && *handle & UINTA_FILE_IS_BUFFERED_MASK)) return false;
  return true;
}

const void* FileManager::getData(const file_t* const handle) const {
  if (!isBuffered(handle)) {
    SPDLOG_WARN("Data for file '{}' was requested before it has been loaded.", getPath(handle));
    return nullptr;
  }
  return links.at(getId(handle)).ptr;
}

std::string FileManager::getDataString(const file_t* const handle) const {
  return {static_cast<const char*>(getData(handle)), getSize(handle)};
}

std::string FileManager::getPath(const file_t* const handle) const {
  return handlePaths.at(getId(handle));
}

const file_size_t FileManager::getSize(const file_t* const handle) const {
  if (!isActive(handle)) return 0;
  return links.at(getId(handle)).size;
}

const file_t FileManager::getId(const file_t* const handle) const {
  if (!isInitialized()) return -1;
  return *handle & UINTA_FILE_ID_MASK;
}

void FileManager::reserveSpace(const file_t* const handle) {
  auto& link = links.at(getId(handle)) = MemoryLink();
  if (!isActive(handle)) return;

  auto hasSpace = true;
  auto size = std::filesystem::file_size(getPath(handle));

  if (storageSize < size) {
    SPDLOG_WARN("File '{}' attempted to reserve more space than is available!", getPath(handle));
    return;
  }

  // it's possible that the head of the buffer has been released; let's try to use it:
  MemoryLink* firstReservedLink = nullptr;
  for (int i = 0; i < links.size(); i++) {
    if (!isBuffered(handles.at(i))) continue;
    if (!firstReservedLink || firstReservedLink->ptr > links.at(i).ptr) {
      firstReservedLink = &links.at(i);
    }
  }
  if (firstReservedLink) {
    auto headSpace = static_cast<const char*>(firstReservedLink->ptr) - static_cast<const char*>(storage);
    if (headSpace > size) {
      firstReservedLink->back = &link;
      link.forward = firstReservedLink;
      link.ptr = storage;
      link.size = size;
      SPDLOG_DEBUG("Reserved {} bytes for '{}'.", getSize(handle), getPath(handle));
      return;
    }
  }

  // find storage between neighbors:
  for (auto& neighbor : links) {
    if (!neighbor.ptr) continue;  // link isn't actively in storage
    if (neighbor.forward) {       // check for orphaned space
      auto freeSpace =
          static_cast<const char*>(neighbor.forward->ptr) - static_cast<const char*>(neighbor.ptr) - neighbor.size - 1;
      if (freeSpace < size) continue;  // not enough space, move on
    } else {                           // handle tail of the buffer
      auto freeSpace =
          static_cast<const char*>(storage) + storageSize - static_cast<const char*>(neighbor.ptr) - neighbor.size - 1;
      if (freeSpace < size) {  // no space at tail
        SPDLOG_WARN("Insufficient space when attempting to allocate memory for file '{}'!", getPath(handle));
        hasSpace = false;
        continue;  // not enough space at tail
      }
    }
    link.ptr = static_cast<char*>(neighbor.ptr) + neighbor.size + 1;
    link.size = size;
    link.back = &neighbor;
    link.forward = neighbor.forward;
    if (link.forward) link.forward->back = &link;
    neighbor.forward = &link;
    SPDLOG_DEBUG("Reserved {} bytes for file '{}'.", getSize(handle), getPath(handle));
    return;
  }

  if (hasSpace) {
    // we're the first element in storage:
    link.ptr = storage;
    link.size = size;
    SPDLOG_DEBUG("Reserved {} bytes for file '{}'.", getSize(handle), getPath(handle));
  }
}

void FileManager::parseFileSearchPaths(const std::string& searchPaths, const char delim) {
  std::string val;
  int start = 0;
  int end = 0;
  fileSearchPaths.clear();
  do {
    end = searchPaths.find(delim, start);
    val = std::string(searchPaths.substr(start, end - start));
    if (val.length()) {
      if (val.at(val.length() - 1) != '/') val = val + '/';
      fileSearchPaths.emplace_back(std::string(val));
      SPDLOG_DEBUG("File search path registered: '{}'.", val);
    }
    start = end + 1;  // +1 for delim
  } while (end != -1);
  if (fileSearchPaths.size() > 0) {
    SPDLOG_INFO("{} file search paths registered.", fileSearchPaths.size());
  } else {
    SPDLOG_WARN("Failed to parse any file search paths from input '{}'!", searchPaths);
  }
}

void FileManager::loadAll() {
  for (auto* handle : handles) {
    loadFile(handle);
  }
}

void FileManager::loadFile(const file_t* const handle) {
  if (!isActive(handle) || isBuffered(handle)) return;
  auto absPath = findPath(getPath(handle));
  if (absPath.empty()) {
    SPDLOG_ERROR("Failed to find file '{}'!", getPath(handle));
    releaseFile(handle);
    return;
  }
  setPath(handle, absPath);
  reserveSpace(handle);
  if (links.at(getId(handle)).ptr == nullptr) {
    SPDLOG_ERROR("Invalid storage pointer for '{}'!", getPath(handle));
    return;
  }
  loadHandleData(handle);
}
void FileManager::loadFile(const std::vector<const file_t*>& handles) {
  for (auto* handle : handles) {
    loadFile(handle);
  }
}

void FileManager::loadHandleData(const file_t* const handle) {
  if (!isActive(handle)) {
    SPDLOG_WARN("Attempted to load handle data on an inactive handle for '{}'.", getPath(handle));
    return;
  }
  loadFileText(handle);
  setIsBuffered(handle, true);
}

void FileManager::loadFileText(const file_t* const handle) {
  std::ifstream stream;
  stream.open(getPath(handle));
  if (!stream) {
    SPDLOG_ERROR("Failed to open file at '{}'.", getPath(handle));
    return;
  }
  auto& link = links.at(getId(handle));
  stream.read(static_cast<char*>(link.ptr), link.size);
  stream.close();
}

void FileManager::setIsActive(const file_t* const handle, const bool isActive) {
  auto* h = handles.at(getId(handle));
  *h &= ~UINTA_FILE_IS_ACTIVE_MASK;
  if (isActive) *h |= UINTA_FILE_IS_ACTIVE_MASK;
}

void FileManager::setIsBuffered(const file_t* const handle, const bool isBuffered) {
  if (!isActive(handle)) return;
  auto* h = handles.at(getId(handle));
  *h &= ~UINTA_FILE_IS_BUFFERED_MASK;
  if (isBuffered) *h |= UINTA_FILE_IS_BUFFERED_MASK;
}

void FileManager::setPath(const file_t* const handle, const std::string& path) {
  if (!isActive(handle)) return;
  handlePaths.at(getId(handle)) = std::string(path);
}

std::string FileManager::findPath(const std::string& path) {
  if (std::ifstream(path)) return path;
  for (const auto& searchPath : fileSearchPaths) {
    std::string absPath = sanitizePath(searchPath, path);
    if (std::ifstream(absPath)) return absPath;
  }
  return "";
}

bool FileManager::isInitialized() const {
  if (!isFlagSet(FILEMANAGER_INITIALIZED, flags)) {
    SPDLOG_WARN("FileManager has not been initialized!");
    return false;
  }
  return true;
}

}  // namespace uinta
