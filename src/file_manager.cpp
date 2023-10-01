#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>
#include <fstream>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/utils/formatters.hpp>

namespace uinta {

inline const file_t UINTA_FILE_ID_MASK = 0xFFFF;
inline const file_t UINTA_FILE_IS_BUFFERED_MASK = 0x70000;
inline const file_t UINTA_FILE_IS_ACTIVE_MASK = 0x80000;

inline const flag_t FILEMANAGER_INITIALIZED = 1 << 0;

FileManager::FileManager(const size_t storageSize) : m_storageSize(storageSize), m_storage(malloc(storageSize)) {
}

FileManager::~FileManager() {
  for (auto* handle : m_handles) {
    releaseFile(handle);
    delete handle;
    handle = nullptr;
  }
  free(m_storage);
  m_storage = nullptr;
  m_storageSize = 0;
}

uinta_error_code FileManager::init(const Runner& runner, const std::string& searchPaths, const char delim) {
  if (!m_logger) m_logger = spdlog::stdout_color_mt(runner.logger()->name() + ":FileManager");
  if (isFlagSet(FILEMANAGER_INITIALIZED, m_flags)) SPDLOG_LOGGER_WARN(logger(), "Too many calls to FileManager::init()!");
  parseFileSearchPaths(searchPaths, delim);
  setFlag(FILEMANAGER_INITIALIZED, true, m_flags);
  loadAll();
  return SUCCESS_EC;
}

const file_t* const FileManager::registerFile(const std::string& relativePath) {
  if (m_handles.size() >= UINTA_FILE_ID_MASK) {
    SPDLOG_LOGGER_CRITICAL(logger(), "Attempt made to exceed maximum handle size of {}", UINTA_FILE_ID_MASK);
    return nullptr;
  }
  auto* handle = m_handles.emplace_back(new file_t(m_handles.size()));
  m_handlePaths.emplace_back(relativePath);
  m_links.emplace_back(MemoryLink());

  releaseFile(handle, true);
  setIsActive(handle, true);

  SPDLOG_LOGGER_DEBUG(logger(), "Registered file '{}'.", relativePath);
  return handle;
}

void FileManager::releaseFile(const file_t* const handle, bool force) {
  if (!force && !isActive(handle)) return;
  if (isBuffered(handle)) {
    SPDLOG_LOGGER_DEBUG(logger(), "Releasing file '{}'.", getPath(handle));
  }
  setPath(handle, "");
  setIsBuffered(handle, false);
  setIsActive(handle, false);
  auto& link = m_links.at(getId(handle));
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
  if (handle == nullptr || getId(handle) > m_handles.size()) {
    SPDLOG_LOGGER_WARN(logger(), "Invalid handle {}!", *handle);
  }
  if (m_handles.at(getId(handle)) != handle) return false;
  return *handle & UINTA_FILE_IS_ACTIVE_MASK;
}

const bool FileManager::isBuffered(const file_t* const handle) const {
  if (!isActive(handle)) return false;
  if (!(isActive(handle) && *handle & UINTA_FILE_IS_BUFFERED_MASK)) return false;
  return true;
}

const void* FileManager::getData(const file_t* const handle) const {
  if (!isBuffered(handle)) {
    SPDLOG_LOGGER_WARN(logger(), "Data for file '{}' was requested before it has been loaded.", getPath(handle));
    return nullptr;
  }
  return m_links.at(getId(handle)).ptr;
}

std::string FileManager::getDataString(const file_t* const handle) const {
  return {static_cast<const char*>(getData(handle)), getSize(handle)};
}

std::string FileManager::getPath(const file_t* const handle) const {
  return m_handlePaths.at(getId(handle));
}

const file_size_t FileManager::getSize(const file_t* const handle) const {
  if (!isActive(handle)) return 0;
  return m_links.at(getId(handle)).size;
}

const file_t FileManager::getId(const file_t* const handle) const {
  if (!isInitialized()) return -1;
  return *handle & UINTA_FILE_ID_MASK;
}

void FileManager::reserveSpace(const file_t* const handle) {
  auto& link = m_links.at(getId(handle)) = MemoryLink();
  if (!isActive(handle)) return;

  auto hasSpace = true;
  auto size = std::filesystem::file_size(getPath(handle));

  if (m_storageSize < size) {
    SPDLOG_LOGGER_WARN(logger(), "File '{}' attempted to reserve more space than is available!", getPath(handle));
    return;
  }

  // it's possible that the head of the buffer has been released; let's try to use it:
  MemoryLink* firstReservedLink = nullptr;
  for (size_t i = 0; i < m_links.size(); i++) {
    if (!isBuffered(m_handles.at(i))) continue;
    if (!firstReservedLink || firstReservedLink->ptr > m_links.at(i).ptr) {
      firstReservedLink = &m_links.at(i);
    }
  }
  if (firstReservedLink) {
    size_t headSpace = static_cast<const char*>(firstReservedLink->ptr) - static_cast<const char*>(m_storage);
    if (headSpace > size) {
      firstReservedLink->back = &link;
      link.forward = firstReservedLink;
      link.ptr = m_storage;
      link.size = size;
      SPDLOG_LOGGER_DEBUG(logger(), "Reserved {} for '{}'.", formatMemory(getSize(handle)), getPath(handle));
      return;
    }
  }

  // find storage between neighbors:
  for (auto& neighbor : m_links) {
    if (!neighbor.ptr) continue;  // link isn't actively in storage
    if (neighbor.forward) {       // check for orphaned space
      auto freeSpace =
          static_cast<const char*>(neighbor.forward->ptr) - static_cast<const char*>(neighbor.ptr) - neighbor.size - 1;
      if (freeSpace < size) continue;  // not enough space, move on
    } else {                           // handle tail of the buffer
      auto freeSpace =
          static_cast<const char*>(m_storage) + m_storageSize - static_cast<const char*>(neighbor.ptr) - neighbor.size - 1;
      if (freeSpace < size) {  // no space at tail
        SPDLOG_LOGGER_WARN(logger(), "Insufficient space when attempting to allocate memory for file '{}'!", getPath(handle));
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
    SPDLOG_LOGGER_DEBUG(logger(), "Reserved {} for file '{}'.", formatMemory(getSize(handle)), getPath(handle));
    return;
  }

  if (hasSpace) {
    // we're the first element in storage:
    link.ptr = m_storage;
    link.size = size;
    SPDLOG_LOGGER_DEBUG(logger(), "Reserved {} for file '{}'.", formatMemory(getSize(handle)), getPath(handle));
  }
}

void FileManager::parseFileSearchPaths(const std::string& searchPaths, const char delim) {
  std::string val;
  i32 start = 0;
  i32 end = 0;
  m_searchPaths.clear();
  do {
    end = searchPaths.find(delim, start);
    val = std::string(searchPaths.substr(start, end - start));
    if (val.length()) {
      if (val.at(val.length() - 1) != '/') val = val + '/';
      m_searchPaths.emplace_back(std::string(val));
      SPDLOG_LOGGER_DEBUG(logger(), "File search path registered: '{}'.", val);
    }
    start = end + 1;  // +1 for delim
  } while (end != -1);
  if (m_searchPaths.size() > 0) {
    SPDLOG_LOGGER_INFO(logger(), "{} file search paths registered.", m_searchPaths.size());
  } else {
    SPDLOG_LOGGER_WARN(logger(), "Failed to parse any file search paths from input '{}'!", searchPaths);
  }
}

void FileManager::loadAll() {
  for (auto* handle : m_handles) {
    loadFile(handle);
  }
}

void FileManager::loadFile(const file_t* const handle) {
  if (!isActive(handle) || isBuffered(handle)) return;
  auto absPath = findPath(getPath(handle));
  if (absPath.empty()) {
    SPDLOG_LOGGER_ERROR(logger(), "Failed to find file '{}'!", getPath(handle));
    releaseFile(handle);
    return;
  }
  setPath(handle, absPath);
  reserveSpace(handle);
  if (m_links.at(getId(handle)).ptr == nullptr) {
    SPDLOG_LOGGER_ERROR(logger(), "Invalid storage pointer for '{}'!", getPath(handle));
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
    SPDLOG_LOGGER_WARN(logger(), "Attempted to load handle data on an inactive handle for '{}'.", getPath(handle));
    return;
  }
  loadFileText(handle);
  setIsBuffered(handle, true);
}

void FileManager::setIsActive(const file_t* const handle, const bool isActive) {
  auto* h = m_handles.at(getId(handle));
  *h &= ~UINTA_FILE_IS_ACTIVE_MASK;
  if (isActive) *h |= UINTA_FILE_IS_ACTIVE_MASK;
}

void FileManager::setIsBuffered(const file_t* const handle, const bool isBuffered) {
  if (!isActive(handle)) return;
  auto* h = m_handles.at(getId(handle));
  *h &= ~UINTA_FILE_IS_BUFFERED_MASK;
  if (isBuffered) *h |= UINTA_FILE_IS_BUFFERED_MASK;
}

void FileManager::setPath(const file_t* const handle, const std::string& path) {
  if (!isActive(handle)) return;
  m_handlePaths.at(getId(handle)) = std::string(path);
}

std::string FileManager::findPath(const std::string& path) {
  if (std::ifstream(std::filesystem::path(path).string())) return path;
  for (const auto& searchPath : m_searchPaths) {
    const std::string absPath = std::filesystem::path(searchPath + path).string();
    if (std::ifstream(absPath)) return absPath;
  }
  return "";
}

bool FileManager::isInitialized() const {
  if (!isFlagSet(FILEMANAGER_INITIALIZED, m_flags)) {
    SPDLOG_LOGGER_WARN(logger(), "FileManager has not been initialized!");
    return false;
  }
  return true;
}

void FileManager_Desktop::loadFileBinary(const file_t* const handle) {
  // TODO:
}
void FileManager_Desktop::loadFileText(const file_t* const handle) {
  std::ifstream stream;
  stream.open(getPath(handle));
  if (!stream) {
    SPDLOG_LOGGER_ERROR(logger(), "Failed to open file at '{}'.", getPath(handle));
    return;
  }
  auto& link = links().at(getId(handle));
  stream.read(static_cast<char*>(link.ptr), link.size);
  stream.close();
}

FileManager::MemoryLink::MemoryLink(const MemoryLink& other) {
  *this = other;
}

FileManager::MemoryLink::MemoryLink(void* ptr, size_t size) : size(size), ptr(ptr) {
  back = nullptr;
  forward = nullptr;
}

FileManager::MemoryLink& FileManager::MemoryLink::operator=(const MemoryLink& other) {
  size = other.size;
  ptr = other.ptr;
  back = other.back;
  forward = other.forward;
  return *this;
}

bool FileManager::MemoryLink::operator==(const MemoryLink& rhs) const {
  return size == rhs.size && ptr == rhs.ptr && back == rhs.back && forward == rhs.forward;
}

}  // namespace uinta
