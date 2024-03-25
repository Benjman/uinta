list(APPEND ENGINE_TEST_SRCS src/app_config_yaml_test.cc)
list(APPEND ENGINE_TEST_SRCS src/component_test.cc)
list(APPEND ENGINE_TEST_SRCS src/engine_test.cc)
list(APPEND ENGINE_TEST_SRCS src/gl/capability_guard_test.cc)
list(APPEND ENGINE_TEST_SRCS src/ring_buffer_test.cc)
list(APPEND ENGINE_TEST_SRCS src/running_average_test.cc)
list(APPEND ENGINE_TEST_SRCS src/vao_test.cc)
list(APPEND ENGINE_TEST_SRCS src/vbo_test.cc)

add_executable(engine_test ${ENGINE_TEST_SRCS})
target_link_libraries(engine_test
  ${UINTA_LIBS}
  uinta::engine
  uinta::platform
  GTest::gtest_main)
target_include_directories(engine_test PRIVATE include ${UINTA_INCLUDES}
  ${UINTA_TEST_INCLUDES})

include(GoogleTest)
gtest_discover_tests(engine_test)
