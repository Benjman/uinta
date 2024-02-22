list(APPEND PLATFORM_TEST_SRCS src/input_test.cc)
list(APPEND PLATFORM_TEST_SRCS src/platform_test.cc)

add_executable(platform_test ${PLATFORM_TEST_SRCS})
target_link_libraries(platform_test
  uinta::platform
  GTest::gtest_main
  ${UINTA_ABSL})

include(GoogleTest)
gtest_discover_tests(platform_test)
