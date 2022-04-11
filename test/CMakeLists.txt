find_package(Threads REQUIRED)

include_directories(../include)

add_executable(test
    ./fontTest.cpp
    ./main.cpp
    ./mat_test.cpp
    ./quadtreeTests.cpp
    ./running_avg.cpp
    ./vecTests.cpp
    )

target_link_libraries(test
    uinta
    EnTT::EnTT
    GTest::GTest
    GTest::Main)

add_test(test test)
