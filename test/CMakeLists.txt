find_package(Threads REQUIRED)

include_directories(../src)

add_executable(test
    ./main.cpp
    ./quadTreeTests.cpp
    ./vecTests.cpp
    )

target_link_libraries(test
    EnTT::EnTT
    GTest::GTest
    GTest::Main)

add_test(test test)
