#ifndef UINTA_METRICS_HPP
#define UINTA_METRICS_HPP

using metric_t = int;

constexpr unsigned int METRIC_FLOAT = 0;
constexpr unsigned int METRIC_INT   = 1;
constexpr unsigned int METRIC_UINT  = 2;

constexpr unsigned int METRICS_MAX_STORAGE = 100;

struct MetricsController final {

  void *storage = nullptr;
  unsigned int metric_type[METRICS_MAX_STORAGE];
  const char *assignments[METRICS_MAX_STORAGE];

  MetricsController();

  ~MetricsController();

  metric_t init_metric(const unsigned int type, const char *const name) noexcept;

  void set(const metric_t handle, const float v) noexcept;
  void set(const metric_t handle, const int v) noexcept;
  void set(const metric_t handle, const unsigned int v) noexcept;

  float getf(const metric_t handle) noexcept;
  int geti(const metric_t handle) noexcept;
  unsigned int getui(const metric_t handle) noexcept;
};

#endif // UINTA_METRICS_HPP
