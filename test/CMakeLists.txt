find_package(Threads REQUIRED)

include_directories(../include)

add_executable(test
    ./main.cpp
    ./mat_test.cpp
    ./quadtreeTests.cpp
    ./vecTests.cpp
    )

target_link_libraries(test
    uinta
    EnTT::EnTT
    GTest::GTest
    GTest::Main)

add_test(test test)
