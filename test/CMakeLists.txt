include_directories(../include)

add_executable(tests
  ./main.cpp


  ./tests/fontTest.cpp
  ./tests/math
  ./tests/metrics_test.cpp
  ./tests/quadtreeTests.cpp
  ./tests/resource.cpp
  ./tests/running_avg.cpp

  )

target_link_libraries(tests uinta

  EnTT::EnTT
  gtest
  gtest_main

  )

add_test(tests tests)
