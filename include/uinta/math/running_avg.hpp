#ifndef UINTA_MATH_RUNNING_AVG_HPP
#define UINTA_MATH_RUNNING_AVG_HPP

namespace uinta {
struct RunningAvg final {
 public:
  unsigned int count;

  explicit RunningAvg(const unsigned int) noexcept;
  RunningAvg(const RunningAvg&) noexcept;
  RunningAvg& operator=(const RunningAvg&) noexcept;

  void operator+=(const float) noexcept;

  ~RunningAvg();

  void add(float) noexcept;

  float avg() noexcept;

 private:
  float* buffer;
  float mavg;
  bool dirty;
  unsigned int position;
};
}  // namespace uinta

#endif  // UINTA_MATH_RUNNING_AVG_HPP
