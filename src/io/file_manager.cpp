#include "uinta/io/file_manager.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include "./utils.hpp"
#include "uinta/cfg.hpp"
#include "uinta/memory/memory_link.hpp"

using namespace uinta;

FileManager::FileManager(const size_t storageSize) : storageSize(storageSize), storage(malloc(storageSize)) {}

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

void FileManager::init(const std::string searchPaths, const char delim) { parseFileSearchPaths(searchPaths, delim, fileSearchPaths); }

const file_t* const FileManager::registerFile(const std::string& relativePath, const FileType type) {
  file_t* handle = handles.emplace_back(new file_t(handles.size()));
  handlePaths.emplace_back(relativePath);
  blockList.emplace_back(MemoryLink());

  releaseFile(handle, true);
  setIsActive(handle, true);
  setType(handle, type);

  SPDLOG_DEBUG("Registered handle {} for '{}'", getId(handle), relativePath);
  return handle;
}

void FileManager::releaseFile(const file_t* const handle, bool force) {
  if (!force && !isActive(handle)) return;
  setPath(handle, "");
  setIsBuffered(handle, false);
  setIsActive(handle, false);
  MemoryLink& block = blockList.at(getId(handle));
  if (block.back) block.back->forward = block.forward;
  if (block.forward) block.forward->back = block.back;
  block = MemoryLink();
}

const bool FileManager::isActive(const file_t* const handle) const {
  if (handles.at(getId(handle)) != handle) return false;
  return *handle & UINTA_FILE_IS_ACTIVE_MASK;
}

const bool FileManager::isBuffered(const file_t* const handle) const {
  return isActive(handle) && *handle & UINTA_FILE_IS_BUFFERED_MASK;
}

const void* FileManager::getData(const file_t* const handle) const { return blockList.at(getId(handle)).ptr; }

const char* FileManager::getDataChars(const file_t* const handle) const { return (char*)getData(handle); }

const std::string& FileManager::getPath(const file_t* const handle) const { return handlePaths.at(getId(handle)); }

const file_size_t FileManager::getSize(const file_t* const handle) const {
  if (!isActive(handle)) return 0;
  return blockList.at(getId(handle)).size;
}

const FileType FileManager::getType(const file_t* const handle) const { return (FileType)(*handle & UINTA_FILE_TYPE_MASK); }

const file_t FileManager::getId(const file_t* const handle) const { return *handle & UINTA_FILE_ID_MASK; }

void FileManager::reserveSpace(const file_t* const handle) {
  auto& block = blockList.at(getId(handle)) = MemoryLink();
  if (!isActive(handle)) return;

  bool hasSpace = true;
  auto size = std::filesystem::file_size(getPath(handle));

  if (storageSize < size) {
    SPDLOG_WARN("File '{}' attempted to reserve more space than is available!", getPath(handle));
    return;
  }

  // It's possible that the head of the buffer has been released, so let's try to use it:
  MemoryLink* frb = nullptr;  // first reserved block
  for (int i = 0; i < blockList.size(); i++) {
    if (!isBuffered(handles.at(i))) continue;
    auto& block = blockList.at(i);
    if (frb == nullptr || frb->ptr > block.ptr) {
      frb = &block;
    }
  }
  if (frb != nullptr) {
    file_size_t headSpace = (char*)frb->ptr - (char*)storage;
    if (headSpace >= size + 1) {
      frb->back = &block;
      block.forward = frb;
      block.ptr = storage;
      block.size = size;
      return;
    }
  }

  // Find storage between neighbors:
  for (auto& b : blockList) {
    if (!b.ptr) continue;                                                      // block isn't actively in storage
    if (b.forward) {                                                           // check for orphaned space
      if ((char*)b.forward->ptr - (char*)b.ptr - b.size - 1 < size) continue;  // not enough space, move on
    } else {                                                                   // handle tail of the buffer
      if ((char*)storage + storageSize - (char*)b.ptr - b.size - 1 < size) {
        SPDLOG_WARN("Insufficient space when attempting to allocate memory for file '{}'!", getPath(handle));
        hasSpace = false;
        continue;  // not enough space at tail
      }
    }
    block.ptr = (char*)b.ptr + b.size + 1;
    block.size = size;
    block.back = &b;
    block.forward = b.forward;
    if (block.forward) block.forward->back = &block;
    b.forward = &block;
    return;
  }

  if (hasSpace) {
    // We're the first element in storage:
    block.ptr = storage;
    block.size = size;
  }
}

void FileManager::loadAll() {
  for (auto* handle : handles) {
    loadHandle(handle);
  }
}

void FileManager::loadHandle(const file_t* const handle) {
  if (!isActive(handle) || isBuffered(handle)) return;
  auto absPath = findPath(getPath(handle), fileSearchPaths);
  if (absPath.empty()) {
    SPDLOG_ERROR("Failed to find file '{}'.", getPath(handle));
    releaseFile(handle);
    return;
  }
  setPath(handle, absPath);
  reserveSpace(handle);
  if (blockList.at(getId(handle)).ptr) loadHandleData(handle);
}

void FileManager::loadHandleData(const file_t* const handle) {
  switch (getType(handle)) {
    case FileType::Binary:
      loadFileBinary(handle);
      break;

    case FileType::Text:
      loadFileText(handle);
      break;

    default: {
      SPDLOG_WARN("Type not found for file_t value {}", *handle);
      return;
    }
  }
  setIsBuffered(handle, true);
}

void FileManager::loadFileBinary(const file_t* const handle) {
  // TODO
}

void FileManager::loadFileText(const file_t* const handle) {
  std::ifstream stream;
  stream.open(getPath(handle));
  if (!stream) {
    SPDLOG_ERROR("Failed to open file at '{}'", getPath(handle));
    return;
  }
  auto& block = blockList.at(getId(handle));
  stream.read(static_cast<char*>(block.ptr), block.size);
  stream.close();
}

void FileManager::setIsActive(const file_t* const handle, const bool isActive) {
  auto* h = handles.at(getId(handle));
  *h &= ~UINTA_FILE_IS_ACTIVE_MASK;
  if (isActive) *h |= UINTA_FILE_IS_ACTIVE_MASK;
}

void FileManager::setIsBuffered(const file_t* const handle, const bool isLoaded) {
  if (!isActive(handle)) return;
  auto* h = handles.at(getId(handle));
  *h &= ~UINTA_FILE_IS_BUFFERED_MASK;
  if (isLoaded) *h |= UINTA_FILE_IS_BUFFERED_MASK;
}

void FileManager::setPath(const file_t* const handle, const std::string& path) {
  if (!isActive(handle)) return;
  handlePaths.at(getId(handle)) = std::string(path);
}

void FileManager::setType(const file_t* const handle, const FileType type) {
  if (!isActive(handle)) return;
  auto* h = handles.at(getId(handle));
  *h &= ~UINTA_FILE_TYPE_MASK;
  *h |= type;
}
