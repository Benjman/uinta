#include "uinta/resource/resource_manager.hpp"

using namespace uinta;

ResourceManager::ResourceBlock::ResourceBlock(const ResourceManager::ResourceBlock& other) { *this = other; }

ResourceManager::ResourceBlock::ResourceBlock(void* ptr, resource_size_t size) : ptr(ptr), size(size) {
  back = nullptr;
  forward = nullptr;
}

ResourceManager::ResourceBlock& ResourceManager::ResourceBlock::operator=(const ResourceManager::ResourceBlock& other) {
  size = other.size;
  ptr = other.ptr;
  back = other.back;
  forward = other.forward;
  return *this;
}

bool ResourceManager::ResourceBlock::operator==(const ResourceManager::ResourceBlock& rhs) const {
  return size == rhs.size && ptr == rhs.ptr && back == rhs.back && forward == rhs.forward;
}
