#ifndef SRC_ENGINE_INCLUDE_UINTA_MATH_RUNNING_AVERAGE_H_
#define SRC_ENGINE_INCLUDE_UINTA_MATH_RUNNING_AVERAGE_H_

#include <numeric>

#include "uinta/data/ring_buffer.h"

namespace uinta {

template <typename T, size_t N>
class RunningAverage final : public RingBuffer<T, N> {
 public:
  T getAverage() const noexcept {
    if (RingBuffer<T, N>::size() > 0) {
      auto accumulation =
          std::accumulate(RingBuffer<T, N>::begin(), RingBuffer<T, N>::end(),
                          static_cast<T>(0));
      auto size = static_cast<T>(RingBuffer<T, N>::size());
      return static_cast<T>(accumulation / size);
    }
    return static_cast<T>(0);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MATH_RUNNING_AVERAGE_H_
