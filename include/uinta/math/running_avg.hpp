#ifndef UINTA_MATH_RUNNING_AVG_HPP
#define UINTA_MATH_RUNNING_AVG_HPP

#include <uinta/types.h>

namespace uinta {
struct RunningAvg final {
 public:
  u32 count;

  explicit RunningAvg(const u32) noexcept;
  RunningAvg(const RunningAvg&) noexcept;
  RunningAvg& operator=(const RunningAvg&) noexcept;

  void operator+=(const f32) noexcept;

  ~RunningAvg();

  void add(f32) noexcept;

  f32 avg() noexcept;

 private:
  f32* buffer;
  f32 mavg;
  bool dirty;
  u32 position;
};
}  // namespace uinta

#endif  // UINTA_MATH_RUNNING_AVG_HPP
