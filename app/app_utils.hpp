#ifndef UINTA_APP_UTILS_HPP
#define UINTA_APP_UTILS_HPP

#include <debug.hpp>
#include <glfw.hpp>


#include "./utils/camera_controller.hpp"
#ifdef UINTA_APP_UTILS_IMPL
#include "./utils/camera_controller.cpp"
#endif // UINTA_APP_UTILS_IMPL


#include "./utils/debug_controller.hpp"
#ifdef UINTA_APP_UTILS_IMPL
#include "./utils/debug_controller.cpp"
#endif // UINTA_APP_UTILS_IMPL


#include "./utils/runner.hpp"
#ifdef UINTA_APP_UTILS_IMPL
#include "./utils/runner.cpp"
#endif // UINTA_APP_UTILS_IMPL


#endif // UINTA_APP_UTILS_HPP
