#ifndef UINTA_APP_UTILS_HPP
#define UINTA_APP_UTILS_HPP

#include <debug.hpp>

#include "./utils/glfw_runner.hpp"
#ifdef UINTA_APP_UTILS_IMPL
#include "./utils/glfw_runner.cpp"
#endif // UINTA_APP_UTILS_IMPL

#include "./utils/camera_controller.hpp"
#ifdef UINTA_APP_UTILS_IMPL
#include "./utils/camera_controller.cpp"
#endif // UINTA_APP_UTILS_IMPL


#include "./utils/debug_controller.hpp"
#ifdef UINTA_APP_UTILS_IMPL
#include "./utils/debug_controller.cpp"
#endif // UINTA_APP_UTILS_IMPL


#endif // UINTA_APP_UTILS_HPP
