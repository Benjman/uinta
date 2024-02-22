list(APPEND PLATFORM_TEST_SRCS src/input_test.cc)
list(APPEND PLATFORM_TEST_SRCS src/platform_test.cc)

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
