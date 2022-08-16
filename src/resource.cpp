#include "uinta/resource.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include "./resource/resource_block.cpp"
#include "./resource/resource_utils.cpp"
#include "./resource/resource_utils.hpp"
#include "uinta/cfg.hpp"

using namespace uinta;

ResourceManager::ResourceManager(const size_t storageSize) : storageSize(storageSize), storage(malloc(storageSize)) {}

ResourceManager::~ResourceManager() {
  for (auto* handle : handles) {
    release(handle);
    delete handle;
    handle = nullptr;
  }
  free(storage);
  storage = nullptr;
  storageSize = 0;
}

void ResourceManager::init(const std::string resPaths, const char delim) { parseResourcePaths(resPaths, delim, resourcePaths); }

const resource_t* const ResourceManager::registerResource(const std::string& relativePath, const ResourceType type) {
  resource_t* handle = handles.emplace_back(new resource_t(handles.size()));  // FIXME new-ing up is awful. Use an allocator.
  handlePaths.emplace_back(relativePath);
  blockList.emplace_back(ResourceBlock());

  release(handle, true);
  setIsActive(handle, true);
  setType(handle, type);

  SPDLOG_DEBUG("Registered resource '{}'", relativePath);
  return handle;
}

const file_t* const ResourceManager::registerFile(const std::string& relativePath, const ResourceType type) {
  return registerResource(relativePath, type);
}

void ResourceManager::release(const resource_t* const handle, bool force) {
  if (!force && !isActive(handle)) return;
  setPath(handle, "");
  setIsBuffered(handle, false);
  setIsActive(handle, false);
  ResourceBlock& block = blockList.at(getId(getHandle(handle)));
  if (block.back) block.back->forward = block.forward;
  if (block.forward) block.forward->back = block.back;
  block = ResourceBlock();
}

const bool ResourceManager::isActive(const resource_t* const handle) const {
  return getHandle(handle) && *handle & UINTA_RES_ACTIVE_MASK;
}

const bool ResourceManager::isBuffered(const resource_t* const handle) const {
  return isActive(handle) && *handle & UINTA_RES_BUFFERED_MASK;
}

const void* ResourceManager::getData(const resource_t* const handle) const { return blockList.at(getId(getHandle(handle))).ptr; }

const char* ResourceManager::getDataChars(const resource_t* const handle) const { return (char*)getData(handle); }

const std::string& ResourceManager::getPath(const resource_t* const handle) const {
  return handlePaths.at(getId(getHandle(handle)));
}

const resource_size_t ResourceManager::getSize(const resource_t* const handle) const {
  if (!isActive(handle)) return 0;
  return blockList.at(getId(getHandle(handle))).size;
}

const ResourceType ResourceManager::getType(const resource_t* const handle) const {
  return (ResourceType)(*handle & UINTA_RES_TYPE_MASK);
}

resource_t* const ResourceManager::getHandle(const resource_t* const handle) const {
  if (handles.at(getId(handle)) != handle) return nullptr;
  return handles.at(getId(handle));
}

const resource_t ResourceManager::getId(const resource_t* const handle) const { return *handle & UINTA_RES_ID_MASK; }

void ResourceManager::reserveSpace(const resource_t* const handle) {
  auto& block = blockList.at(getId(getHandle(handle))) = ResourceBlock();
  if (!isActive(handle)) return;

  bool hasSpace = true;
  auto size = std::filesystem::file_size(getPath(handle));

  if (storageSize < size) {
    SPDLOG_WARN("Resource '{}' attempted to reserve more space than is available!", getPath(handle));
    return;
  }

  // It's possible that the head of the buffer has been released, so let's try to use it:
  ResourceBlock* frb = nullptr;  // first reserved block
  for (int i = 0; i < blockList.size(); i++) {
    if (!isBuffered(handles.at(i))) continue;
    auto& block = blockList.at(i);
    if (frb == nullptr || frb->ptr > block.ptr) {
      frb = &block;
    }
  }
  if (frb != nullptr) {
    resource_size_t headSpace = (char*)frb->ptr - (char*)storage;
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
        SPDLOG_WARN("No space left for resource '{}'!", getPath(handle));
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

void ResourceManager::loadAll() {
  for (auto* handle : handles) {
    if (!isActive(handle) || isBuffered(handle)) continue;
    SPDLOG_DEBUG("Attempting to load resource '{}'", getPath(handle));
    loadHandle(handle);
    SPDLOG_DEBUG("Loaded {} bytes for resource '{}'", getSize(handle), getPath(handle));
  }
}

void ResourceManager::loadHandle(const resource_t* const handle) {
  if (!isActive(handle) || isBuffered(handle)) return;
  auto absPath = findPath(getPath(handle), resourcePaths);
  if (absPath.empty()) {
    SPDLOG_ERROR("Failed to find resource '{}'.", getPath(handle));
    release(handle);
    return;
  }
  setPath(handle, absPath);
  SPDLOG_DEBUG("Found resource path '{}'", getPath(handle));
  reserveSpace(handle);
  if (blockList.at(getId(getHandle(handle))).ptr) loadHandleData(handle);
}

void ResourceManager::loadHandleData(const resource_t* const handle) {
  switch (getType(handle)) {
    case ResourceType::Binary:
      loadResourceBinary(handle);
      break;

    case ResourceType::Text:
      loadResourceText(handle);
      break;

    default: {
      SPDLOG_WARN("Type not found for resource_t value {}", *handle);
      return;
    }
  }
  setIsBuffered(handle, true);
}

void ResourceManager::loadResourceBinary(const resource_t* const handle) {
  // TODO
}

void ResourceManager::loadResourceText(const resource_t* const handle) {
  std::ifstream stream;
  stream.open(getPath(handle));
  if (!stream) {
    SPDLOG_ERROR("Failed to open file at '{}'", getPath(handle));
    return;
  }
  auto& block = blockList.at(getId(getHandle(handle)));
  stream.read(static_cast<char*>(block.ptr), block.size);
  stream.close();
}

void ResourceManager::setIsActive(const resource_t* const handle, const bool isActive) {
  auto* h = getHandle(handle);
  *h &= ~UINTA_RES_ACTIVE_MASK;
  if (isActive) *h |= UINTA_RES_ACTIVE_MASK;
}

void ResourceManager::setIsBuffered(const resource_t* const handle, const bool isLoaded) {
  if (!isActive(handle)) return;
  auto* h = getHandle(handle);
  *h &= ~UINTA_RES_BUFFERED_MASK;
  if (isLoaded) *h |= UINTA_RES_BUFFERED_MASK;
}

void ResourceManager::setPath(const resource_t* const handle, const std::string& path) {
  if (!isActive(handle)) return;
  handlePaths.at(getId(getHandle(handle))) = std::string(path);
}

void ResourceManager::setType(const resource_t* const handle, const ResourceType type) {
  if (!isActive(handle)) return;
  auto* h = getHandle(handle);
  *h &= ~UINTA_RES_TYPE_MASK;
  *h |= type;
}
