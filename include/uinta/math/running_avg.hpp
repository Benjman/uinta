#ifndef UINTA_MATH_RUNNING_AVG_HPP
#define UINTA_MATH_RUNNING_AVG_HPP

#include <uinta/types.h>

namespace uinta {
struct RunningAvg final {
 public:
  explicit RunningAvg(const u32 sample_size) noexcept {
    buffer = new f32[sample_size];
    mavg = 0.0;
    dirty = false;
    position = 0u;
    count = sample_size;
  }

  void operator+=(const f32 v) noexcept {
    add(v);
  }

  ~RunningAvg() {
    delete[] buffer;
  }

  void add(f32 v) noexcept {
    buffer[position % count] = v;
    dirty = true;
    position++;
    // TODO check for cursor violating u32 max
  }

  f32 avg() noexcept {
    if (!position) return 0.0;
    if (dirty) {
      f32 sum = 0.0;
      u32 len = std::min(position, count);
      for (u32 i = 0u; i < len; i++) {
        sum += buffer[i];
      }
      mavg = sum / (f32)len;
      dirty = false;
    }
    return mavg;
  }

 private:
  f32* buffer;
  u32 count;
  bool dirty;
  f32 mavg;
  u32 position;
};
}  // namespace uinta

#endif  // UINTA_MATH_RUNNING_AVG_HPP
