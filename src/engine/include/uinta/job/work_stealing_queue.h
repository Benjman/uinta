#ifndef SRC_ENGINE_INCLUDE_UINTA_JOB_WORK_STEALING_QUEUE_H_
#define SRC_ENGINE_INCLUDE_UINTA_JOB_WORK_STEALING_QUEUE_H_

#include <atomic>
#include <cassert>
#include <optional>
#include <vector>

#include "uinta/types.h"

namespace uinta {

// A lock-free work-stealing deque based on the Chase-Lev algorithm.
// The owning thread pushes and pops from the bottom (LIFO).
// Other threads steal from the top (FIFO).
//
// This implementation is based on "Dynamic Circular Work-Stealing Deque"
// by Chase and Lev, and adapted from Anthony Williams' implementation
// in "C++ Concurrency in Action".
template <typename T>
class WorkStealingQueue {
 public:
  explicit WorkStealingQueue(size_t capacity)
      : buffer_(new CircularBuffer(capacity)), top_(0), bottom_(0) {}

  ~WorkStealingQueue() { delete buffer_.load(std::memory_order_relaxed); }

  WorkStealingQueue(const WorkStealingQueue&) = delete;
  WorkStealingQueue& operator=(const WorkStealingQueue&) = delete;

  WorkStealingQueue(WorkStealingQueue&&) = delete;
  WorkStealingQueue& operator=(WorkStealingQueue&&) = delete;

  // Push an item to the bottom of the deque.
  // Only called by the owning thread.
  // Returns false if the queue is full.
  bool push(T item) noexcept {
    auto b = bottom_.load(std::memory_order_relaxed);
    auto t = top_.load(std::memory_order_acquire);
    auto* buf = buffer_.load(std::memory_order_relaxed);

    if (static_cast<size_t>(b - t) >= buf->capacity()) {
      // Queue is full - could grow here, but for simplicity we reject
      return false;
    }

    buf->set(b, std::move(item));
    std::atomic_thread_fence(std::memory_order_release);
    bottom_.store(b + 1, std::memory_order_relaxed);
    return true;
  }

  // Pop an item from the bottom of the deque.
  // Only called by the owning thread.
  std::optional<T> pop() noexcept {
    auto b = bottom_.load(std::memory_order_relaxed) - 1;
    auto* buf = buffer_.load(std::memory_order_relaxed);
    bottom_.store(b, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto t = top_.load(std::memory_order_relaxed);

    if (t <= b) {
      // Non-empty queue
      T item = buf->get(b);
      if (t == b) {
        // Last item - race with steal
        if (!top_.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst,
                                          std::memory_order_relaxed)) {
          // Lost race
          bottom_.store(b + 1, std::memory_order_relaxed);
          return std::nullopt;
        }
        bottom_.store(b + 1, std::memory_order_relaxed);
      }
      return item;
    }
    // Empty queue
    bottom_.store(b + 1, std::memory_order_relaxed);
    return std::nullopt;
  }

  // Steal an item from the top of the deque.
  // Called by other threads.
  std::optional<T> steal() noexcept {
    auto t = top_.load(std::memory_order_acquire);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto b = bottom_.load(std::memory_order_acquire);

    if (t < b) {
      // Non-empty queue
      auto* buf = buffer_.load(std::memory_order_consume);
      T item = buf->get(t);
      if (!top_.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst,
                                        std::memory_order_relaxed)) {
        // Lost race with another steal or pop
        return std::nullopt;
      }
      return item;
    }
    return std::nullopt;
  }

  // Returns true if the queue appears empty.
  // This is a best-effort check and may be stale.
  [[nodiscard]] bool empty() const noexcept {
    auto b = bottom_.load(std::memory_order_relaxed);
    auto t = top_.load(std::memory_order_relaxed);
    return b <= t;
  }

  // Returns the approximate number of items in the queue.
  [[nodiscard]] size_t size() const noexcept {
    auto b = bottom_.load(std::memory_order_relaxed);
    auto t = top_.load(std::memory_order_relaxed);
    return b > t ? static_cast<size_t>(b - t) : 0;
  }

 private:
  // Circular buffer for storing items
  class CircularBuffer {
   public:
    explicit CircularBuffer(size_t cap)
        : capacity_(cap), mask_(cap - 1), buffer_(cap) {
      // Capacity must be a power of 2 for efficient modulo
      assert((cap & (cap - 1)) == 0 && "Capacity must be a power of 2");
    }

    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }

    void set(i64 index, T item) noexcept {
      buffer_[static_cast<size_t>(index) & mask_] = std::move(item);
    }

    T get(i64 index) const noexcept {
      return buffer_[static_cast<size_t>(index) & mask_];
    }

   private:
    size_t capacity_;
    size_t mask_;
    std::vector<T> buffer_;
  };

  std::atomic<CircularBuffer*> buffer_;
  std::atomic<i64> top_;
  std::atomic<i64> bottom_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_JOB_WORK_STEALING_QUEUE_H_
