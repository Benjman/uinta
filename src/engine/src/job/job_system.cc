#include "uinta/job/job_system.h"

#include "absl/log/log.h"

namespace uinta {

// Thread-local storage definitions
thread_local i32 JobSystem::localThreadIndex_ = -1;
thread_local JobSystem::Queue* JobSystem::localQueue_ = nullptr;

JobSystem::JobSystem(JobSystemParams params) noexcept {
  // Ensure buffer capacity is a power of 2
  size_t capacity = params.bufferCapacity;
  if (capacity == 0) {
    capacity = 256;
  }
  // Round up to next power of 2 if needed
  if ((capacity & (capacity - 1)) != 0) {
    size_t power = 1;
    while (power < capacity) {
      power <<= 1;
    }
    capacity = power;
  }

  // Determine thread count
  size_t threadCount = params.threadCount;
  if (threadCount == 0) {
    threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) {
      threadCount = 4;  // Fallback
    }
  }

  LOG(INFO) << "JobSystem initializing with " << threadCount
            << " worker threads, buffer capacity " << capacity;

  // Create work-stealing queues for each worker
  queues_.reserve(threadCount);
  for (size_t i = 0; i < threadCount; ++i) {
    queues_.push_back(std::make_unique<Queue>(capacity));
  }

  // Start worker threads (but not for index 0 - that's the main thread)
  workers_.reserve(threadCount - 1);
  for (size_t i = 1; i < threadCount; ++i) {
    workers_.emplace_back([this, i](const std::stop_token& /*token*/) {
      localThreadIndex_ = static_cast<i32>(i);
      localQueue_ = queues_[i].get();
      workerLoop();
    });
  }

  // Set up main thread as worker 0
  localThreadIndex_ = 0;
  localQueue_ = queues_[0].get();
}

JobSystem::~JobSystem() noexcept {
  // Signal shutdown
  shutdown_.store(true, std::memory_order_release);

  // Wake up all waiting workers
  {
    std::lock_guard<std::mutex> lock(notifyMutex_);
    notifyCV_.notify_all();
  }

  // jthreads will auto-join on destruction
  workers_.clear();

  LOG(INFO) << "JobSystem shutdown complete";
}

void JobSystem::workerLoop() noexcept {
  while (!shutdown_.load(std::memory_order_acquire)) {
    // Try to execute a job
    if (runPendingJob()) {
      continue;
    }

    // No work available - wait for notification
    std::unique_lock<std::mutex> lock(notifyMutex_);
    notifyCV_.wait_for(lock, std::chrono::milliseconds(1), [this]() {
      return shutdown_.load(std::memory_order_acquire) ||
             pendingJobs_.load(std::memory_order_acquire) > 0;
    });
  }

  // Drain remaining jobs before exiting
  while (runPendingJob()) {
  }
}

bool JobSystem::runPendingJob() noexcept {
  // First try local queue
  if (tryExecuteLocalJob()) {
    return true;
  }

  // Then try global queue
  {
    std::unique_lock<std::mutex> lock(globalMutex_);
    if (!globalQueue_.empty()) {
      Job job = std::move(globalQueue_.back());
      globalQueue_.pop_back();
      lock.unlock();

      job();
      auto remaining = pendingJobs_.fetch_sub(1, std::memory_order_acq_rel) - 1;
      if (remaining == 0) {
        std::lock_guard<std::mutex> waitLock(waitMutex_);
        waitCV_.notify_all();
      }
      return true;
    }
  }

  // Finally try stealing from other workers
  return tryStealJob();
}

bool JobSystem::tryExecuteLocalJob() noexcept {
  Queue* queue = getLocalQueue();
  if (queue == nullptr) {
    return false;
  }

  auto job = queue->pop();
  if (job) {
    (*job)();
    auto remaining = pendingJobs_.fetch_sub(1, std::memory_order_acq_rel) - 1;
    if (remaining == 0) {
      std::lock_guard<std::mutex> lock(waitMutex_);
      waitCV_.notify_all();
    }
    return true;
  }
  return false;
}

bool JobSystem::tryStealJob() noexcept {
  if (queues_.empty()) {
    return false;
  }

  // Start from a pseudo-random index to reduce contention
  size_t start = static_cast<size_t>(localThreadIndex_ + 1) % queues_.size();
  size_t count = queues_.size();

  for (size_t i = 0; i < count; ++i) {
    size_t idx = (start + i) % count;
    if (static_cast<i32>(idx) == localThreadIndex_) {
      continue;
    }

    auto job = queues_[idx]->steal();
    if (job) {
      (*job)();
      auto remaining = pendingJobs_.fetch_sub(1, std::memory_order_acq_rel) - 1;
      if (remaining == 0) {
        std::lock_guard<std::mutex> lock(waitMutex_);
        waitCV_.notify_all();
      }
      return true;
    }
  }
  return false;
}

JobSystem::Queue* JobSystem::getLocalQueue() noexcept { return localQueue_; }

void JobSystem::submitInternal(Job job) noexcept {
  // Try to push to local queue first
  Queue* queue = getLocalQueue();
  if ((queue != nullptr) && queue->push(std::move(job))) {
    // Wake up a worker
    std::lock_guard<std::mutex> lock(notifyMutex_);
    notifyCV_.notify_one();
    return;
  }

  // Fall back to global queue (from non-worker threads or if local is full)
  {
    std::lock_guard<std::mutex> lock(globalMutex_);
    globalQueue_.push_back(std::move(job));
  }

  // Wake up a worker
  {
    std::lock_guard<std::mutex> lock(notifyMutex_);
    notifyCV_.notify_one();
  }
}

void JobSystem::waitAll() noexcept {
  // Run jobs while waiting
  while (pendingJobs_.load(std::memory_order_acquire) > 0) {
    if (!runPendingJob()) {
      // No local work - wait briefly for other workers
      std::unique_lock<std::mutex> lock(waitMutex_);
      waitCV_.wait_for(lock, std::chrono::microseconds(100), [this]() {
        return pendingJobs_.load(std::memory_order_acquire) == 0;
      });
    }
  }
}

i32 JobSystem::threadIndex() noexcept { return localThreadIndex_; }

}  // namespace uinta
