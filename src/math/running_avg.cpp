#include <algorithm>
#include <uinta/math/running_avg.hpp>

using namespace uinta;

RunningAvg::RunningAvg(const u32 sample_size) noexcept {
  buffer = new f32[sample_size];
  mavg = 0.0;
  dirty = false;
  position = 0u;
  count = sample_size;
}

RunningAvg::RunningAvg(const RunningAvg& other) noexcept {
  *this = other;
}

RunningAvg& RunningAvg::operator=(const RunningAvg& other) noexcept {
  buffer = other.buffer;
  mavg = other.mavg;
  dirty = other.dirty;
  position = other.position;
  count = other.count;
  return *this;
}

void RunningAvg::operator+=(const f32 v) noexcept {
  add(v);
}

RunningAvg::~RunningAvg() {
  delete[] buffer;
}

f32 RunningAvg::avg() noexcept {
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

void RunningAvg::add(f32 v) noexcept {
  buffer[position % count] = v;
  dirty = true;
  position++;
  // TODO check for cursor violating u32 max
}
