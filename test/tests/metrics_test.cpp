#include <string>
#include <uinta/metrics.hpp>

#include <gtest/gtest.h>

using namespace uinta;

// TEST(metrics, initial_state) {
//   auto m = MetricsController();
//   for (unsigned int i = 0; i < METRICS_MAX_STORAGE; i++)
//     ASSERT_EQ(0, m.assignments[i]) << "Failed on iteration " << std::to_string(i);
// }

TEST(metrics, init_metric) {
  auto m = MetricsController();
  ASSERT_NE(METRIC_FLOAT, m.metric_type[0]);
  metric_t handle = m.init_metric(METRIC_FLOAT, "testing");
  ASSERT_EQ(0, handle);
  ASSERT_STREQ("testing", m.assignments[0]);
  ASSERT_EQ(METRIC_FLOAT, m.metric_type[0]);
}

TEST(metrics, set_float) {
  auto m          = MetricsController();
  metric_t handle = m.init_metric(METRIC_FLOAT, "testing");
  m.set(handle, 0.1f);
  float v = *(float *)(m.storage);
  ASSERT_EQ(0.1f, v);
}

TEST(metrics, set_int) {
  auto m          = MetricsController();
  metric_t handle = m.init_metric(METRIC_INT, "testing");
  m.set(handle, 3);
  int v = *(int *)(m.storage);
  ASSERT_EQ(3, v);
}

TEST(metrics, set_uint) {
  auto m          = MetricsController();
  metric_t handle = m.init_metric(METRIC_INT, "testing");
  m.set(handle, 3u);
  int v = *(int *)(m.storage);
  ASSERT_EQ(3u, v);
}

TEST(metrics, getf) {
  auto m          = MetricsController();
  metric_t handle = m.init_metric(METRIC_UINT, "testing");
  m.set(handle, 0.1f);
  ASSERT_EQ(0.1f, m.getf(handle));
}

TEST(metrics, geti) {
  auto m          = MetricsController();
  metric_t handle = m.init_metric(METRIC_FLOAT, "testing");
  m.set(handle, 5);
  ASSERT_EQ(5, m.geti(handle));
}

TEST(metrics, getui) {
  auto m          = MetricsController();
  metric_t handle = m.init_metric(METRIC_FLOAT, "testing");
  m.set(handle, 5u);
  ASSERT_EQ(5u, m.geti(handle));
}

TEST(metrics, second_position) {
  auto m     = MetricsController();
  metric_t a = m.init_metric(METRIC_FLOAT, "testing");
  m.set(a, 3.0f);
  ASSERT_EQ(3.0f, m.getf(a));
  metric_t b = m.init_metric(METRIC_FLOAT, "testing");
  m.set(b, 13);
  ASSERT_EQ(13, m.getui(b));
}
