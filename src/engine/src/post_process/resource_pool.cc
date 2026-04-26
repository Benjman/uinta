#include "uinta/post_process/resource_pool.h"

namespace uinta {

namespace {

bool SameFormat(const TextureFormat& a, const TextureFormat& b) noexcept {
  return a.internalFormat == b.internalFormat &&
         a.pixelFormat == b.pixelFormat && a.pixelType == b.pixelType &&
         a.levels == b.levels && a.immutable == b.immutable;
}

}  // namespace

bool ResourceDesc::matches(const RenderTarget* rt) const noexcept {
  if (rt == nullptr) {
    return false;
  }
  if (rt->width() != width || rt->height() != height) {
    return false;
  }
  if (!SameFormat(rt->colorFormat(), color)) {
    return false;
  }
  const bool haveDepth = depth.has_value();
  const bool rtHasDepth = rt->depthFormat().has_value();
  if (haveDepth != rtHasDepth) {
    return false;
  }
  if (haveDepth && !SameFormat(*rt->depthFormat(), *depth)) {
    return false;
  }
  return true;
}

RenderTarget* ResourcePool::acquire(const ResourceDesc& desc) noexcept {
  // Find a free cached entry that matches the descriptor.
  for (auto& entry : entries_) {
    if (!entry.inUse && desc.matches(entry.target.get())) {
      entry.inUse = true;
      return entry.target.get();
    }
  }

  // Allocate a fresh one.
  Entry entry;
  entry.desc = desc;
  entry.target = std::make_unique<RenderTarget>(
      gl_, desc.width, desc.height, desc.color, desc.depth, desc.debugName);
  entry.inUse = true;
  entries_.push_back(std::move(entry));
  return entries_.back().target.get();
}

void ResourcePool::release(RenderTarget* target) noexcept {
  for (auto& entry : entries_) {
    if (entry.target.get() == target) {
      entry.inUse = false;
      return;
    }
  }
}

void ResourcePool::resetActive() noexcept {
  for (auto& entry : entries_) {
    entry.inUse = false;
  }
}

void ResourcePool::invalidate() noexcept { entries_.clear(); }

}  // namespace uinta
