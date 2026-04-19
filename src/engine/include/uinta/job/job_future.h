#ifndef SRC_ENGINE_INCLUDE_UINTA_JOB_JOB_FUTURE_H_
#define SRC_ENGINE_INCLUDE_UINTA_JOB_JOB_FUTURE_H_

#include <atomic>
#include <exception>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>
#include <variant>

namespace uinta {

class JobSystem;

namespace detail {

// Shared state between JobFuture and the job execution
template <typename T>
struct JobState {
  std::atomic<bool> ready{false};
  std::variant<std::monostate, T, std::exception_ptr> result;
  JobSystem* jobSystem = nullptr;

  void setResult(T value) noexcept {
    result = std::move(value);
    ready.store(true, std::memory_order_release);
  }

  void setException(std::exception_ptr e) noexcept {
    result = e;
    ready.store(true, std::memory_order_release);
  }

  [[nodiscard]] bool isReady() const noexcept {
    return ready.load(std::memory_order_acquire);
  }

  T getResult() {
    if (std::holds_alternative<std::exception_ptr>(result)) {
      std::rethrow_exception(std::get<std::exception_ptr>(result));
    }
    return std::move(std::get<T>(result));
  }
};

// Specialization for void
template <>
struct JobState<void> {
  std::atomic<bool> ready{false};
  std::exception_ptr exception;
  JobSystem* jobSystem = nullptr;

  void setResult() noexcept { ready.store(true, std::memory_order_release); }

  void setException(std::exception_ptr e) noexcept {
    exception = std::move(e);
    ready.store(true, std::memory_order_release);
  }

  [[nodiscard]] bool isReady() const noexcept {
    return ready.load(std::memory_order_acquire);
  }

  void getResult() const {
    if (exception) {
      std::rethrow_exception(exception);
    }
  }
};

}  // namespace detail

// A future representing the result of an asynchronously executed job.
// Waiting on this future runs other pending jobs ("productive spin")
// rather than blocking the thread.
template <typename T>
class JobFuture {
 public:
  JobFuture() = default;

  explicit JobFuture(std::shared_ptr<detail::JobState<T>> state) noexcept
      : state_(std::move(state)) {}

  JobFuture(const JobFuture&) = delete;
  JobFuture& operator=(const JobFuture&) = delete;

  JobFuture(JobFuture&&) noexcept = default;
  JobFuture& operator=(JobFuture&&) noexcept = default;

  ~JobFuture() = default;

  // Check if the result is ready without blocking.
  [[nodiscard]] bool isReady() const noexcept {
    return state_ && state_->isReady();
  }

  // Wait for the result, running other jobs while waiting.
  // Throws if the job threw an exception.
  T wait();

  // Returns true if this future is valid (has associated state).
  explicit operator bool() const noexcept { return state_ != nullptr; }

 private:
  std::shared_ptr<detail::JobState<T>> state_;
};

// Specialization for void
template <>
class JobFuture<void> {
 public:
  JobFuture() = default;

  explicit JobFuture(std::shared_ptr<detail::JobState<void>> state) noexcept
      : state_(std::move(state)) {}

  JobFuture(const JobFuture&) = delete;
  JobFuture& operator=(const JobFuture&) = delete;

  JobFuture(JobFuture&&) noexcept = default;
  JobFuture& operator=(JobFuture&&) noexcept = default;

  ~JobFuture() = default;

  [[nodiscard]] bool isReady() const noexcept {
    return state_ && state_->isReady();
  }

  // Declared here, defined in job_future.cc to avoid circular dependency
  void wait();

  explicit operator bool() const noexcept { return state_ != nullptr; }

 private:
  std::shared_ptr<detail::JobState<void>> state_;
};

// Template implementation for non-void types
template <typename T>
T JobFuture<T>::wait() {
  if (!state_) {
    throw std::runtime_error("JobFuture::wait() called on invalid future");
  }

  // Productive spin - run other jobs while waiting
  while (!state_->isReady()) {
    if (state_->jobSystem) {
      if (!state_->jobSystem->runPendingJob()) {
        // No work available - yield to avoid spinning
        std::this_thread::yield();
      }
    } else {
      std::this_thread::yield();
    }
  }

  return state_->getResult();  // May throw stored exception
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_JOB_JOB_FUTURE_H_
