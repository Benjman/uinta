#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_RESOURCE_POOL_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_RESOURCE_POOL_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "uinta/gl.h"
#include "uinta/post_process/render_target.h"
#include "uinta/texture.h"
#include "uinta/types.h"

namespace uinta {

/*! Descriptor used to key transient render-target allocations. */
struct ResourceDesc {
  u32 width = 0;
  u32 height = 0;
  TextureFormat color;
  std::optional<TextureFormat> depth;
  std::string debugName;

  [[nodiscard]] bool matches(const RenderTarget* rt) const noexcept;
};

/*! Lightweight pool of transient `RenderTarget`s.
 *
 *  Not a full frame graph: passes explicitly `acquire` a target for the
 *  duration of their execution and `release` it back when done. The pool
 *  caches targets across frames so typical per-frame allocations cost
 *  nothing after warm-up. Viewport resizes drop stale entries via
 *  `invalidate()`.
 *
 *  Lifetime rule: acquired targets must be released within the same frame;
 *  the PostProcessor calls `resetActive()` after each chain execution to
 *  catch leaks (all pool-owned targets become available for reuse again).
 */
class ResourcePool {
 public:
  explicit ResourcePool(const OpenGLApi* gl) noexcept : gl_(gl) {}

  ResourcePool(const ResourcePool&) = delete;
  ResourcePool& operator=(const ResourcePool&) = delete;
  ResourcePool(ResourcePool&&) = delete;
  ResourcePool& operator=(ResourcePool&&) = delete;

  /*! Retrieve a render target matching `desc`. Allocates a new one if no
   *  suitable cached entry is available. Returned target is "in use" and
   *  will not be returned by a subsequent `acquire` until `release()` or
   *  `resetActive()` is called. */
  RenderTarget* acquire(const ResourceDesc& desc) noexcept;

  /*! Return a previously-acquired target to the pool for reuse. */
  void release(RenderTarget* target) noexcept;

  /*! Mark all currently-acquired targets as free without deallocating. Meant
   *  to be called once per frame by the PostProcessor after chain execution,
   *  so targets from the previous frame are available for the current. */
  void resetActive() noexcept;

  /*! Drop all cached targets (e.g. on viewport resize). */
  void invalidate() noexcept;

  /*! Number of cached targets currently held. */
  [[nodiscard]] size_t size() const noexcept { return entries_.size(); }

 private:
  struct Entry {
    ResourceDesc desc;
    std::unique_ptr<RenderTarget> target;
    bool inUse = false;
  };

  const OpenGLApi* gl_;
  std::vector<Entry> entries_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_RESOURCE_POOL_H_
