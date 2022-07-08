include_directories(../include)

add_executable(tests
  ./main.cpp

  # ./fontTest.cpp
  # ./math/mat_test.cpp
  # ./math/vecTests.cpp
  # ./metrics_test.cpp
  # ./quadtreeTests.cpp

  ./running_avg.cpp
  ./math/smooth_float_test.cpp

  )

target_link_libraries(tests uinta

  EnTT::EnTT
  gtest
  gtest_main

  )

add_test(tests tests)
