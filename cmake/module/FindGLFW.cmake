find_package(glad REQUIRED)
find_library(GLFW_LIBRARY
    NAMES glfw glfw3
    PATHS
    /usr/lib
    /usr/local/lib)

set(GL_LIBS ${GL_LIBS} ${GLFW_LIBRARY})
if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    set(GL_LIBS
        ${GL_LIBS}
        ${LIBRARIES}
        ${X11_LIBRARIES}
        ${X11_Xrandr_LIB}
        ${CMAKE_DL_LIBS}
        Threads::Threads)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY)

