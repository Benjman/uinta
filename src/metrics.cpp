#include <metrics.hpp>

#include <cstdio>
#include <algorithm>
#include <cstring>

constexpr unsigned int METRICS_STRIDE = sizeof(float);
constexpr metric_t METRICS_INVALID = -1;

metrics_controller::metrics_controller() noexcept {
    storage = malloc(METRICS_MAX_STORAGE * METRICS_STRIDE);
    memset(assignments, 0, METRICS_MAX_STORAGE * METRICS_STRIDE);
}

metrics_controller::~metrics_controller() {
    free(storage);
}

metric_t metrics_controller::init_metric(const unsigned int type, const char* const name) noexcept {
    for (int i = 0; i < METRICS_MAX_STORAGE; i++) {
        if (!assignments[i]) {
            metric_type[i] = type;
            assignments[i] = name;
            set(i, 0);
            return i;
        }
    }
    printf("Metrics storage full: Ignoring metric '%s'\n", name);
    return METRICS_INVALID;
}

void metrics_controller::set(const metric_t handle, const float v) noexcept {
    auto* position = (float*) storage + handle * METRICS_STRIDE;
    *position = v;
}

void metrics_controller::set(const metric_t handle, const int v) noexcept {
    auto* position = (int*) storage + handle * METRICS_STRIDE;
    *position = v;
}

void metrics_controller::set(const metric_t handle, const unsigned int v) noexcept {
    // void* position = storage + handle * METRICS_STRIDE;
    auto* position = static_cast<unsigned int*>(storage) + 4;
    *position = v;
}

float metrics_controller::getf(const metric_t handle) noexcept {
    if (handle <= METRICS_INVALID) {
        printf("metrics_controller.getf() received an invalid handle\n");
        return 0.0f;
    }
    auto* position = (float*) storage + handle * METRICS_STRIDE;
    return *position;
}

int metrics_controller::geti(const metric_t handle) noexcept {
    if (handle <= METRICS_INVALID) {
        printf("metrics_controller.getf() received an invalid handle\n");
        return 0;
    }
    auto* position = (int*) storage + handle * METRICS_STRIDE;
    return *position;
}

unsigned int metrics_controller::getui(const metric_t handle) noexcept {
    if (handle <= METRICS_INVALID) {
        printf("metrics_controller.getf() received an invalid handle\n");
        return 0u;
    }
    auto* position = (unsigned int*) storage + handle * METRICS_STRIDE;
    return *position;
}