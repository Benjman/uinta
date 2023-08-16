#include <spdlog/spdlog.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <uinta/logging.hpp>
#include <uinta/metrics.hpp>

using namespace uinta;

constexpr u32 METRICS_STRIDE = sizeof(f32);
constexpr metric_t METRICS_INVALID = -1;

#define METRICS_VALIDATE_STRIDE(type) \
  if (METRICS_STRIDE != sizeof(type)) \
  throw std::runtime_error(std::string("[ERROR] metrics_controller.cpp - unexpected return from sizeof(") + #type + ")")

MetricsController::MetricsController() {
  // validate stride is correct
  METRICS_VALIDATE_STRIDE(i32);
  METRICS_VALIDATE_STRIDE(u32);
  METRICS_VALIDATE_STRIDE(f32);

  storage = malloc(METRICS_MAX_STORAGE * METRICS_STRIDE);
  memset(assignments, 0, METRICS_MAX_STORAGE * METRICS_STRIDE);
}

MetricsController::~MetricsController() {
  free(storage);
}

metric_t MetricsController::init_metric(const u32 type, const char* const name) noexcept {
  for (u32 i = 0; i < METRICS_MAX_STORAGE; i++) {
    if (!assignments[i]) {
      metric_type[i] = type;
      assignments[i] = name;
      set(i, 0);
      return i;
    }
  }
  SPDLOG_ERROR("Metrics storage full: Ignoring metric '{}'", name);
  return METRICS_INVALID;
}

void MetricsController::set(const metric_t handle, const f32 v) noexcept {
  auto* position = static_cast<f32*>(storage) + handle * METRICS_STRIDE;
  *position = v;
}

void MetricsController::set(const metric_t handle, const i32 v) noexcept {
  auto* position = static_cast<i32*>(storage) + handle * METRICS_STRIDE;
  *position = v;
}

void MetricsController::set(const metric_t handle, const u32 v) noexcept {
  auto* position = static_cast<u32*>(storage) + handle * METRICS_STRIDE;
  *position = v;
}

f32 MetricsController::getf(const metric_t handle) noexcept {
  if (handle <= METRICS_INVALID) {
    SPDLOG_ERROR("metrics_controller.getf() received an invalid handle");
    return 0.0f;
  }
  auto* position = static_cast<f32*>(storage) + handle * METRICS_STRIDE;
  return *position;
}

i32 MetricsController::geti(const metric_t handle) noexcept {
  if (handle <= METRICS_INVALID) {
    SPDLOG_ERROR("metrics_controller.getf() received an invalid handle");
    return 0;
  }
  auto* position = static_cast<i32*>(storage) + handle * METRICS_STRIDE;
  return *position;
}

u32 MetricsController::getui(const metric_t handle) noexcept {
  if (handle <= METRICS_INVALID) {
    SPDLOG_ERROR("metrics_controller.getf() received an invalid handle");
    return 0u;
  }
  auto* position = static_cast<u32*>(storage) + handle * METRICS_STRIDE;
  return *position;
}
