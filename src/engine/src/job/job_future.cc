#include "uinta/job/job_future.h"

#include <thread>

#include "uinta/job/job_system.h"

namespace uinta {

void JobFuture<void>::wait() {
  if (!state_) {
    return;
  }

  // Productive spin - run other jobs while waiting
  while (!state_->isReady()) {
    if (state_->jobSystem != nullptr) {
      if (!state_->jobSystem->runPendingJob()) {
        // No work available - yield to avoid spinning
        std::this_thread::yield();
      }
    } else {
      std::this_thread::yield();
    }
  }

  state_->getResult();  // May throw stored exception
}

}  // namespace uinta
