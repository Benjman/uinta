#ifndef SRC_ENGINE_INCLUDE_UINTA_JOB_JOB_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_JOB_JOB_SYSTEM_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "uinta/job/job_future.h"
#include "uinta/job/work_stealing_queue.h"
#include "uinta/types.h"

namespace uinta {

// Callbacks for fire-and-forget jobs
struct JobCallbacks {
  std::function<void()> onSuccess = nullptr;
  std::function<void(std::exception_ptr)> onError = nullptr;
};

// Callbacks for jobs that return a value
template <typename T>
struct JobCallbacksWithResult {
  std::function<void(T&)> onSuccess = nullptr;
  std::function<void(std::exception_ptr)> onError = nullptr;
};

// Parameters for JobSystem initialization
struct JobSystemParams {
  size_t bufferCapacity = 256;  // Jobs per worker buffer (must be power of 2)
  size_t threadCount = 0;       // 0 = std::thread::hardware_concurrency()
};

// A work-stealing job system for parallel task execution.
//
// Design based on Anthony Williams' thread pool from "C++ Concurrency in
// Action":
// - One worker thread per CPU core (main thread is also a worker)
// - Thread-local job buffers with work stealing for load balancing
// - Non-blocking futures that run pending jobs while waiting
//
// Thread safety: All public methods are thread-safe.
class JobSystem {
 public:
  explicit JobSystem(JobSystemParams params = {}) noexcept;

  ~JobSystem() noexcept;

  JobSystem(const JobSystem&) = delete;
  JobSystem& operator=(const JobSystem&) = delete;
  JobSystem(JobSystem&&) = delete;
  JobSystem& operator=(JobSystem&&) = delete;

  // Submit a job that returns a value.
  // Returns a future that can be waited on.
  template <typename F>
  auto submit(F&& job) -> JobFuture<std::invoke_result_t<F>>;

  // Submit a job with callbacks for fire-and-forget patterns.
  template <typename F>
  auto submit(F&& job, JobCallbacks callbacks)
      -> JobFuture<std::invoke_result_t<F>>;

  // Submit a job that returns a value, with typed callbacks.
  // Only available for jobs that return non-void.
  template <typename F, typename R = std::invoke_result_t<F>,
            std::enable_if_t<!std::is_void_v<R>, int> = 0>
  auto submit(F&& job, JobCallbacksWithResult<R> callbacks) -> JobFuture<R>;

  // Parallel iteration over a range.
  // Divides work into chunks and processes them in parallel.
  template <typename Iterator, typename Func>
  void parallelFor(Iterator begin, Iterator end, Func&& func);

  // Run one pending job on the current thread.
  // Returns true if a job was executed, false if no jobs were available.
  // Useful for doing work while waiting on futures.
  bool runPendingJob() noexcept;

  // Wait for all submitted jobs to complete.
  // This is used at stage boundaries to ensure all work is done.
  void waitAll() noexcept;

  // Get the current worker thread index.
  // Returns -1 if called from a non-worker thread.
  static i32 threadIndex() noexcept;

  // Get the number of worker threads (including main thread).
  [[nodiscard]] size_t threadCount() const noexcept { return queues_.size(); }

 private:
  using Job = std::function<void()>;
  using Queue = WorkStealingQueue<Job>;

  void workerLoop() noexcept;
  bool tryExecuteLocalJob() noexcept;
  bool tryStealJob() noexcept;
  Queue* getLocalQueue() noexcept;
  void submitInternal(Job job) noexcept;

  std::vector<std::jthread> workers_;
  std::vector<std::unique_ptr<Queue>> queues_;

  // Global queue for jobs submitted from non-worker threads
  std::mutex globalMutex_;
  std::vector<Job> globalQueue_;

  // Notification for waiting workers
  std::mutex notifyMutex_;
  std::condition_variable notifyCV_;

  // Shutdown flag
  std::atomic<bool> shutdown_{false};

  // Counter for pending jobs (for waitAll)
  std::atomic<size_t> pendingJobs_{0};
  std::mutex waitMutex_;
  std::condition_variable waitCV_;

  // Thread-local storage for worker index
  static thread_local i32 localThreadIndex_;
  static thread_local Queue* localQueue_;
};

// Template implementations

template <typename F>
auto JobSystem::submit(F&& job) -> JobFuture<std::invoke_result_t<F>> {
  using R = std::invoke_result_t<F>;

  auto state = std::make_shared<detail::JobState<R>>();
  state->jobSystem = this;

  pendingJobs_.fetch_add(1, std::memory_order_relaxed);

  if constexpr (std::is_void_v<R>) {
    submitInternal([state, f = std::forward<F>(job)]() mutable {
      try {
        f();
        state->setResult();
      } catch (...) {
        state->setException(std::current_exception());
      }
    });
  } else {
    submitInternal([state, f = std::forward<F>(job)]() mutable {
      try {
        state->setResult(f());
      } catch (...) {
        state->setException(std::current_exception());
      }
    });
  }

  return JobFuture<R>(state);
}

template <typename F>
auto JobSystem::submit(F&& job, JobCallbacks callbacks)
    -> JobFuture<std::invoke_result_t<F>> {
  using R = std::invoke_result_t<F>;
  static_assert(std::is_void_v<R>,
                "Use JobCallbacksWithResult for jobs that return a value");

  auto state = std::make_shared<detail::JobState<R>>();
  state->jobSystem = this;

  pendingJobs_.fetch_add(1, std::memory_order_relaxed);

  submitInternal(
      [state, f = std::forward<F>(job), cb = std::move(callbacks)]() mutable {
        try {
          f();
          state->setResult();
          if (cb.onSuccess) {
            try {
              cb.onSuccess();
            } catch (...) {  // NOLINT
              // Callback threw - log and swallow to prevent crashing job system
            }
          }
        } catch (...) {
          auto e = std::current_exception();
          state->setException(e);
          if (cb.onError) {
            try {
              cb.onError(e);
            } catch (...) {  // NOLINT
              // Callback threw - swallow
            }
          }
        }
      });

  return JobFuture<R>(state);
}

template <typename F, typename R, std::enable_if_t<!std::is_void_v<R>, int>>
auto JobSystem::submit(F&& job, JobCallbacksWithResult<R> callbacks)
    -> JobFuture<R> {
  auto state = std::make_shared<detail::JobState<R>>();
  state->jobSystem = this;

  pendingJobs_.fetch_add(1, std::memory_order_relaxed);

  submitInternal(
      [state, f = std::forward<F>(job), cb = std::move(callbacks)]() mutable {
        try {
          R result = f();
          state->setResult(result);
          if (cb.onSuccess) {
            try {
              cb.onSuccess(result);
            } catch (...) {  // NOLINT
              // Callback threw - swallow
            }
          }
        } catch (...) {
          auto e = std::current_exception();
          state->setException(e);
          if (cb.onError) {
            try {
              cb.onError(e);
            } catch (...) {  // NOLINT
              // Callback threw - swallow
            }
          }
        }
      });

  return JobFuture<R>(state);
}

template <typename Iterator, typename Func>
void JobSystem::parallelFor(Iterator begin, Iterator end, Func&& func) {
  auto distance = std::distance(begin, end);
  if (distance <= 0) return;

  size_t count = static_cast<size_t>(distance);
  size_t numWorkers = workers_.size();
  if (numWorkers == 0) {
    numWorkers = 1;
  }

  // Determine chunk size - at least 1 item per chunk
  size_t chunkSize = (count + numWorkers - 1) / numWorkers;
  if (chunkSize == 0) {
    chunkSize = 1;
  }

  std::vector<JobFuture<void>> futures;
  futures.reserve((count + chunkSize - 1) / chunkSize);

  auto it = begin;
  while (it != end) {
    auto chunkEnd = it;
    auto remaining = static_cast<size_t>(std::distance(it, end));
    size_t thisChunk = std::min(chunkSize, remaining);
    std::advance(
        chunkEnd,
        static_cast<typename std::iterator_traits<Iterator>::difference_type>(
            thisChunk));

    futures.push_back(submit([it, chunkEnd, &func]() {
      for (auto current = it; current != chunkEnd; ++current) {
        func(*current);
      }
    }));

    it = chunkEnd;
  }

  // Wait for all chunks to complete
  for (auto& future : futures) {
    future.wait();
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_JOB_JOB_SYSTEM_H_
