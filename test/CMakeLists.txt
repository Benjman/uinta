include_directories(../include)

add_executable(tests ./main.cpp

  ./tests/fontTest.cpp
  ./tests/math.cpp
  ./tests/metrics_test.cpp
  ./tests/quadtreeTests.cpp
  ./tests/fileManagerTests.cpp
  ./tests/running_avg.cpp
  ./tests/utils/formatters.cpp

  )

target_link_libraries(tests uinta

  EnTT::EnTT
  gtest
  gtest_main

  )

add_test(tests tests)

message(STATUS "Adding ${CMAKE_CURRENT_LIST_DIR}/res to \$\{UINTA_RES_PATHS\}")
set(UINTA_RES_PATHS ${UINTA_RES_PATHS} ${CMAKE_CURRENT_LIST_DIR}/res PARENT_SCOPE)
