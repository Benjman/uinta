#ifndef UINTA_LOGGING_HPP
#define UINTA_LOGGING_HPP

///
/// spdlog levels for reference:
///
// #define SPDLOG_LEVEL_TRACE 0
// #define SPDLOG_LEVEL_DEBUG 1
// #define SPDLOG_LEVEL_INFO 2
// #define SPDLOG_LEVEL_WARN 3
// #define SPDLOG_LEVEL_ERROR 4
// #define SPDLOG_LEVEL_CRITICAL 5
// #define SPDLOG_LEVEL_OFF 6

#ifdef SPDLOG_ACTIVE_LEVEL
#undef SPDLOG_ACTIVE_LEVEL
#endif  // SPDLOG_ACTIVE_LEVEL

// // trace
// #define SPDLOG_ACTIVE_LEVEL 0

// debug
#define SPDLOG_ACTIVE_LEVEL 1

// // info
// #define SPDLOG_ACTIVE_LEVEL 2

// // warn
// #define SPDLOG_ACTIVE_LEVEL 3

// // error
// #define SPDLOG_ACTIVE_LEVEL 4

// // critical
// #define SPDLOG_ACTIVE_LEVEL 5

// // off
// #define SPDLOG_ACTIVE_LEVEL 6

#include <spdlog/spdlog.h>
using logger_t = std::shared_ptr<spdlog::logger>;

#endif  // !UINTA_LOGGING_HPP
