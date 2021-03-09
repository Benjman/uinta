cmake_minimum_required(VERSION 3.17)

project(UintaDesktop
        VERSION 0.1)

set(SOURCES ${SOURCES}
        main.cpp
        glfw.cpp)

find_package(GLFW REQUIRED)
set(LIBRARIES ${LIBRARIES} ${GLFW_LIB})
set(INCLUDES ${INCLUDES} ${GLFW_INCLUDE})

if (UNIX AND NOT APPLE)
    find_package(GLAD REQUIRED)
    set(INCLUDES ${INCLUDES} ${GLAD_INCLUDE})
    set(SOURCES ${SOURCES} ${GLAD_C})

    find_package(X11 REQUIRED)
    set(LIBRARIES ${LIBRARIES}
            ${X11_LIBRARIES}
            ${X11_Xrandr_LIB}
            ${CMAKE_DL_LIBS})
endif ()

add_executable(UintaDesktop ${SOURCES})
include_directories(${INCLUDES})
target_link_libraries(UintaDesktop ${LIBRARIES} UintaCore)
