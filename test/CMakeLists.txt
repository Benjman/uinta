find_package(Threads REQUIRED)

include_directories(../include)

add_executable(tests
    ./fontTest.cpp
    ./main.cpp
    ./math/mat_test.cpp
    ./math/vecTests.cpp
    ./metrics_test.cpp
    ./quadtreeTests.cpp
    ./running_avg.cpp
    )

target_link_libraries(tests
    uinta
    EnTT::EnTT
    GTest::GTest
    GTest::Main)

add_test(tests tests)
