#include <spdlog/spdlog.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <uinta/logging.hpp>
#include <uinta/metrics.hpp>

using namespace uinta;

constexpr unsigned int METRICS_STRIDE = sizeof(float);
constexpr metric_t METRICS_INVALID = -1;

#define METRICS_VALIDATE_STRIDE(type) \
  if (METRICS_STRIDE != sizeof(type)) \
  throw std::runtime_error(std::string("[ERROR] metrics_controller.cpp - unexpected return from sizeof(") + #type + ")")

MetricsController::MetricsController() {
  // validate stride is correct
  METRICS_VALIDATE_STRIDE(int);
  METRICS_VALIDATE_STRIDE(unsigned int);
  METRICS_VALIDATE_STRIDE(float);

  storage = malloc(METRICS_MAX_STORAGE * METRICS_STRIDE);
  memset(assignments, 0, METRICS_MAX_STORAGE * METRICS_STRIDE);
}

MetricsController::~MetricsController() {
  free(storage);
}

metric_t MetricsController::init_metric(const unsigned int type, const char* const name) noexcept {
  for (int i = 0; i < METRICS_MAX_STORAGE; i++) {
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

void MetricsController::set(const metric_t handle, const float v) noexcept {
  auto* position = static_cast<float*>(storage) + handle * METRICS_STRIDE;
  *position = v;
}

void MetricsController::set(const metric_t handle, const int v) noexcept {
  auto* position = static_cast<int*>(storage) + handle * METRICS_STRIDE;
  *position = v;
}

void MetricsController::set(const metric_t handle, const unsigned int v) noexcept {
  auto* position = static_cast<unsigned int*>(storage) + handle * METRICS_STRIDE;
  *position = v;
}

float MetricsController::getf(const metric_t handle) noexcept {
  if (handle <= METRICS_INVALID) {
    SPDLOG_ERROR("metrics_controller.getf() received an invalid handle");
    return 0.0f;
  }
  auto* position = static_cast<float*>(storage) + handle * METRICS_STRIDE;
  return *position;
}

int MetricsController::geti(const metric_t handle) noexcept {
  if (handle <= METRICS_INVALID) {
    SPDLOG_ERROR("metrics_controller.getf() received an invalid handle");
    return 0;
  }
  auto* position = static_cast<int*>(storage) + handle * METRICS_STRIDE;
  return *position;
}

unsigned int MetricsController::getui(const metric_t handle) noexcept {
  if (handle <= METRICS_INVALID) {
    SPDLOG_ERROR("metrics_controller.getf() received an invalid handle");
    return 0u;
  }
  auto* position = static_cast<unsigned int*>(storage) + handle * METRICS_STRIDE;
  return *position;
}
