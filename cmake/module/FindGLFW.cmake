find_package(glad REQUIRED)

# GLFW is required to be on the system
# TODO windows paths?
find_library(GLFW_LIBRARY
    NAMES glfw glfw3
    PATHS /usr/lib
          /usr/local/lib)

set(UINTA_LIBS ${UINTA_LIBS} ${GLFW_LIBRARY})

if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    set(UINTA_LIBS ${UINTA_LIBS}
        ${LIBRARIES}
        ${X11_LIBRARIES}
        ${X11_Xrandr_LIB}
        ${CMAKE_DL_LIBS}
        Threads::Threads)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY)
