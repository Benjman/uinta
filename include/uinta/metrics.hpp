#ifndef UINTA_METRICS_HPP
#define UINTA_METRICS_HPP

#include <uinta/types.h>

namespace uinta {

using metric_t = i32;

constexpr u32 METRIC_FLOAT = 0;
constexpr u32 METRIC_INT = 1;
constexpr u32 METRIC_UINT = 2;

constexpr u32 METRICS_MAX_STORAGE = 100;

struct MetricsController final {
  void *storage = nullptr;
  u32 metric_type[METRICS_MAX_STORAGE];
  const char *assignments[METRICS_MAX_STORAGE];

  MetricsController();

  ~MetricsController();

  metric_t init_metric(const u32 type, const char *const name) noexcept;

  void set(const metric_t handle, const f32 v) noexcept;
  void set(const metric_t handle, const i32 v) noexcept;
  void set(const metric_t handle, const u32 v) noexcept;

  f32 getf(const metric_t handle) noexcept;
  i32 geti(const metric_t handle) noexcept;
  u32 getui(const metric_t handle) noexcept;
};

}  // namespace uinta

#endif  // UINTA_METRICS_HPP
