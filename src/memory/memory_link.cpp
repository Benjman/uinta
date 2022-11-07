#include <uinta/memory/memory_link.hpp>

using namespace uinta;

MemoryLink::MemoryLink(const MemoryLink& other) {
  *this = other;
}

MemoryLink::MemoryLink(void* ptr, size_t size) : ptr(ptr), size(size) {
  back = nullptr;
  forward = nullptr;
}

MemoryLink& MemoryLink::operator=(const MemoryLink& other) {
  size = other.size;
  ptr = other.ptr;
  back = other.back;
  forward = other.forward;
  return *this;
}

bool MemoryLink::operator==(const MemoryLink& rhs) const {
  return size == rhs.size && ptr == rhs.ptr && back == rhs.back && forward == rhs.forward;
}
