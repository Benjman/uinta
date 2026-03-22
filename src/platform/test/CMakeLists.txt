list(APPEND PLATFORM_TEST_SRCS
  src/args_test.cc
  src/desktop_platform_test.cc
  src/desktop_window_test.cc
  src/file_test.cc
  src/gl_test.cc
  src/input_test.cc
  src/input_token_test.cc
  src/monitor_test.cc
  src/platform_test.cc
  src/platform_test.cc
  src/window_test.cc
)

add_executable(platform_test ${PLATFORM_TEST_SRCS})

target_include_directories(platform_test PRIVATE include ${UINTA_INCLUDES})

target_link_libraries(platform_test
  uinta::platform
  GTest::gtest_main
  ${UINTA_ABSL})

set(UINTA_TEST_INCLUDES
    ${UINTA_TEST_INCLUDES}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PARENT_SCOPE)

include(GoogleTest)
gtest_discover_tests(platform_test)
