list(APPEND ENGINE_TEST_SRCS src/scene_test.cc)
list(APPEND ENGINE_TEST_SRCS src/vbo_test.cc)
list(APPEND ENGINE_TEST_SRCS src/vao_test.cc)

add_executable(engine_test ${ENGINE_TEST_SRCS})
target_link_libraries(engine_test uinta::engine uinta::platform GTest::gtest_main)
target_include_directories(engine_test PRIVATE include)

include(GoogleTest)
gtest_discover_tests(engine_test)
