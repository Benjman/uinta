#include <uinta/utils/running_avg.hpp>

#include <algorithm>

using namespace uinta;

RunningAvg::RunningAvg(const unsigned int sample_size) noexcept {
  buffer   = new float[sample_size];
  mavg     = 0.0;
  dirty    = false;
  position = 0u;
  count    = sample_size;
}

RunningAvg::RunningAvg(const RunningAvg &other) noexcept { *this = other; }

RunningAvg &RunningAvg::operator=(const RunningAvg &other) noexcept {
  buffer   = other.buffer;
  mavg     = other.mavg;
  dirty    = other.dirty;
  position = other.position;
  count    = other.count;
  return *this;
}

void RunningAvg::operator+=(const float v) noexcept { add(v); }

RunningAvg::~RunningAvg() { delete[] buffer; }

float RunningAvg::avg() noexcept {
  if (!position)
    return 0.0;
  if (dirty) {
    float sum        = 0.0;
    unsigned int len = std::min(position, count);
    for (unsigned int i = 0u; i < len; i++) {
      sum += buffer[i];
    }
    mavg  = sum / (float)len;
    dirty = false;
  }
  return mavg;
}

void RunningAvg::add(float v) noexcept {
  buffer[position % count] = v;
  dirty                    = true;
  position++;
  // TODO check for cursor violating uint max
}
