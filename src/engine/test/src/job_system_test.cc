#include "uinta/job/job_system.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <numeric>
#include <thread>
#include <vector>

#include "./utils.h"
#include "uinta/job/work_stealing_queue.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

// =============================================================================
// WorkStealingQueue Tests
// =============================================================================

class WorkStealingQueueTest : public ::testing::Test {};

TEST_F(WorkStealingQueueTest, PushAndPopSingleItem) {
  WorkStealingQueue<int> queue(16);

  EXPECT_TRUE(queue.push(42));

  auto result = queue.pop();
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(42, *result);
}

TEST_F(WorkStealingQueueTest, PopFromEmptyReturnsNullopt) {
  WorkStealingQueue<int> queue(16);

  auto result = queue.pop();
  EXPECT_FALSE(result.has_value());
}

TEST_F(WorkStealingQueueTest, StealFromEmptyReturnsNullopt) {
  WorkStealingQueue<int> queue(16);

  auto result = queue.steal();
  EXPECT_FALSE(result.has_value());
}

TEST_F(WorkStealingQueueTest, LIFOOrder) {
  WorkStealingQueue<int> queue(16);

  queue.push(1);
  queue.push(2);
  queue.push(3);

  // Pop should return in LIFO order (from bottom)
  EXPECT_EQ(3, *queue.pop());
  EXPECT_EQ(2, *queue.pop());
  EXPECT_EQ(1, *queue.pop());
}

TEST_F(WorkStealingQueueTest, StealFIFOOrder) {
  WorkStealingQueue<int> queue(16);

  queue.push(1);
  queue.push(2);
  queue.push(3);

  // Steal should return in FIFO order (from top)
  EXPECT_EQ(1, *queue.steal());
  EXPECT_EQ(2, *queue.steal());
  EXPECT_EQ(3, *queue.steal());
}

TEST_F(WorkStealingQueueTest, EmptyAfterAllPopped) {
  WorkStealingQueue<int> queue(16);

  queue.push(1);
  queue.push(2);

  queue.pop();
  queue.pop();

  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(0u, queue.size());
}

TEST_F(WorkStealingQueueTest, SizeTracking) {
  WorkStealingQueue<int> queue(16);

  EXPECT_EQ(0u, queue.size());
  EXPECT_TRUE(queue.empty());

  queue.push(1);
  EXPECT_EQ(1u, queue.size());
  EXPECT_FALSE(queue.empty());

  queue.push(2);
  EXPECT_EQ(2u, queue.size());

  queue.pop();
  EXPECT_EQ(1u, queue.size());
}

TEST_F(WorkStealingQueueTest, FullQueueRejectsPush) {
  WorkStealingQueue<int> queue(4);  // Small capacity

  EXPECT_TRUE(queue.push(1));
  EXPECT_TRUE(queue.push(2));
  EXPECT_TRUE(queue.push(3));
  EXPECT_TRUE(queue.push(4));
  EXPECT_FALSE(queue.push(5));  // Should fail - queue is full
}

TEST_F(WorkStealingQueueTest, ConcurrentSteal) {
  WorkStealingQueue<int> queue(256);
  constexpr int kItemCount = 100;

  // Push items
  for (int i = 0; i < kItemCount; ++i) {
    queue.push(i);
  }

  std::atomic<int> stealCount{0};
  std::vector<std::thread> stealers;
  stealers.reserve(4);

  for (int t = 0; t < 4; ++t) {
    stealers.emplace_back([&queue, &stealCount]() {
      while (true) {
        auto result = queue.steal();
        if (result.has_value()) {
          stealCount++;
        } else {
          break;
        }
      }
    });
  }

  for (auto& t : stealers) {
    t.join();
  }

  // All items should have been stolen
  EXPECT_EQ(kItemCount, stealCount.load());
  EXPECT_TRUE(queue.empty());
}

// =============================================================================
// JobSystem Tests
// =============================================================================

class JobSystemTest : public UintaTestF {};

TEST_F(JobSystemTest, Construction) {
  JobSystem jobs({.bufferCapacity = 64, .threadCount = 2});
  EXPECT_EQ(2u, jobs.threadCount());
}

TEST_F(JobSystemTest, DefaultThreadCount) {
  JobSystem jobs;
  EXPECT_GT(jobs.threadCount(), 0u);
}

TEST_F(JobSystemTest, SubmitAndWaitVoidJob) {
  JobSystem jobs({.threadCount = 2});

  std::atomic<bool> executed{false};

  auto future = jobs.submit([&executed]() { executed.store(true); });

  future.wait();

  EXPECT_TRUE(executed.load());
}

TEST_F(JobSystemTest, SubmitAndWaitWithResult) {
  JobSystem jobs({.threadCount = 2});

  auto future = jobs.submit([]() { return 42; });

  int result = future.wait();

  EXPECT_EQ(42, result);
}

TEST_F(JobSystemTest, MultipleJobs) {
  JobSystem jobs({.threadCount = 4});

  std::atomic<int> counter{0};
  std::vector<JobFuture<void>> futures;
  futures.reserve(100);

  for (int i = 0; i < 100; ++i) {
    futures.push_back(jobs.submit([&counter]() { counter++; }));
  }

  for (auto& f : futures) {
    f.wait();
  }

  EXPECT_EQ(100, counter.load());
}

TEST_F(JobSystemTest, JobsRunInParallel) {
  JobSystem jobs({.threadCount = 4});

  std::atomic<int> concurrentCount{0};
  std::atomic<int> maxConcurrent{0};

  std::vector<JobFuture<void>> futures;
  futures.reserve(10);

  for (int i = 0; i < 10; ++i) {
    futures.push_back(jobs.submit([&concurrentCount, &maxConcurrent]() {
      int current = ++concurrentCount;
      int expected = maxConcurrent.load();
      while (current > expected &&
             !maxConcurrent.compare_exchange_weak(expected, current)) {
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      --concurrentCount;
    }));
  }

  for (auto& f : futures) {
    f.wait();
  }

  // With 4 threads, we should see some concurrency
  EXPECT_GT(maxConcurrent.load(), 1);
}

TEST_F(JobSystemTest, WaitAll) {
  JobSystem jobs({.threadCount = 4});

  std::atomic<int> counter{0};

  for (int i = 0; i < 50; ++i) {
    jobs.submit([&counter]() {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      counter++;
    });
  }

  jobs.waitAll();

  EXPECT_EQ(50, counter.load());
}

TEST_F(JobSystemTest, ParallelFor) {
  JobSystem jobs({.threadCount = 4});

  std::vector<int> values(100, 0);

  jobs.parallelFor(values.begin(), values.end(), [](int& v) { v = 1; });

  int sum = std::accumulate(values.begin(), values.end(), 0);
  EXPECT_EQ(100, sum);
}

TEST_F(JobSystemTest, ParallelForWithIndex) {
  JobSystem jobs({.threadCount = 4});

  std::vector<std::atomic<int>> values(100);
  for (auto& v : values) {
    v.store(0);
  }

  std::vector<int> indices(100);
  std::iota(indices.begin(), indices.end(), 0);

  jobs.parallelFor(indices.begin(), indices.end(),
                   [&values](int idx) { values[idx].store(idx * 2); });

  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(i * 2, values[i].load());
  }
}

TEST_F(JobSystemTest, ParallelForEmptyRange) {
  JobSystem jobs({.threadCount = 2});

  std::vector<int> values;

  // Should not crash on empty range
  jobs.parallelFor(values.begin(), values.end(), [](int&) {});
}

TEST_F(JobSystemTest, ExceptionPropagation) {
  JobSystem jobs({.threadCount = 2});

  auto future =
      jobs.submit([]() -> int { throw std::runtime_error("test exception"); });

  EXPECT_THROW(future.wait(), std::runtime_error);
}

TEST_F(JobSystemTest, VoidExceptionPropagation) {
  JobSystem jobs({.threadCount = 2});

  auto future =
      jobs.submit([]() { throw std::runtime_error("test exception"); });

  EXPECT_THROW(future.wait(), std::runtime_error);
}

TEST_F(JobSystemTest, CallbackOnSuccess) {
  JobSystem jobs({.threadCount = 2});

  std::atomic<bool> callbackCalled{false};

  auto future =
      jobs.submit([]() {}, JobCallbacks{.onSuccess = [&callbackCalled]() {
                    callbackCalled.store(true);
                  }});

  future.wait();

  // Give callback a chance to execute
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_TRUE(callbackCalled.load());
}

TEST_F(JobSystemTest, CallbackOnError) {
  JobSystem jobs({.threadCount = 2});

  std::atomic<bool> errorCallbackCalled{false};

  auto future = jobs.submit(
      []() { throw std::runtime_error("test"); },
      JobCallbacks{
          .onError = [&errorCallbackCalled](const std::exception_ptr&) {
            errorCallbackCalled.store(true);
          }});

  try {
    future.wait();
  } catch (...) {  // NOLINT
    // Expected
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_TRUE(errorCallbackCalled.load());
}

TEST_F(JobSystemTest, CallbackWithResultOnSuccess) {
  JobSystem jobs({.threadCount = 2});

  std::atomic<int> receivedValue{0};

  auto future = jobs.submit(
      []() { return 42; },
      JobCallbacksWithResult<int>{
          .onSuccess = [&receivedValue](int& val) { receivedValue.store(val); },
      });

  int result = future.wait();
  EXPECT_EQ(42, result);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(42, receivedValue.load());
}

TEST_F(JobSystemTest, ThreadIndex) {
  JobSystem jobs({.threadCount = 4});

  // Main thread should be worker 0
  i32 mainThreadIndex = JobSystem::threadIndex();
  EXPECT_EQ(0, mainThreadIndex);

  // Just verify that thread index works in jobs
  std::atomic<int> seenIndex{-1};
  auto future = jobs.submit(
      [&seenIndex]() { seenIndex.store(JobSystem::threadIndex()); });
  future.wait();

  // Worker thread should have a valid index (0 to threadCount-1)
  EXPECT_GE(seenIndex.load(), 0);
  EXPECT_LT(seenIndex.load(), static_cast<int>(jobs.threadCount()));
}

TEST_F(JobSystemTest, RunPendingJobFromMainThread) {
  JobSystem jobs({.threadCount = 1});  // Only main thread

  std::atomic<bool> executed{false};

  jobs.submit([&executed]() { executed.store(true); });

  // Run jobs on main thread
  while (!executed.load()) {
    jobs.runPendingJob();
  }

  EXPECT_TRUE(executed.load());
}

TEST_F(JobSystemTest, IsReadyBeforeAndAfterCompletion) {
  JobSystem jobs({.threadCount = 2});

  std::atomic<bool> canProceed{false};

  auto future = jobs.submit([&canProceed]() {
    while (!canProceed.load()) {
      std::this_thread::yield();
    }
    return 123;
  });

  // Future should not be ready yet
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_FALSE(future.isReady());

  canProceed.store(true);
  int result = future.wait();

  EXPECT_TRUE(future.isReady());
  EXPECT_EQ(123, result);
}

// =============================================================================
// Engine Integration Tests
// =============================================================================

class JobSystemEngineTest : public UintaTestF {};

TEST_F(JobSystemEngineTest, EngineHasJobSystem) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  ASSERT_NE(nullptr, engine.jobs());
}

TEST_F(JobSystemEngineTest, JobSystemIsRegisteredAsService) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  auto* jobsViaService = engine.service<JobSystem>();
  auto* jobsDirect = engine.jobs();

  EXPECT_EQ(jobsDirect, jobsViaService);
  EXPECT_NE(nullptr, jobsViaService);
}

TEST_F(JobSystemEngineTest, SubmitJobViaEngine) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  std::atomic<bool> executed{false};

  auto future = engine.jobs()->submit([&executed]() { executed.store(true); });

  future.wait();

  EXPECT_TRUE(executed.load());
}

}  // namespace uinta
