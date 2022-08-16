#ifndef UINTA_MEMORY_MEMORY_LINK_HPP
#define UINTA_MEMORY_MEMORY_LINK_HPP

#include <cstddef>

namespace uinta {

struct MemoryLink {
  size_t size;
  void* ptr;
  MemoryLink *back, *forward;

  MemoryLink(void* ptr = nullptr, size_t size = 0);

  MemoryLink(const MemoryLink& other);

  MemoryLink& operator=(const MemoryLink& other);

  bool operator==(const MemoryLink& rhs) const;
};

}  // namespace uinta

#endif  // UINTA_MEMORY_MEMORY_LINK_HPP
