#ifndef UINTA_LOGGING_HPP
#define UINTA_LOGGING_HPP

#ifdef SPDLOG_ACTIVE_LEVEL
#undef SPDLOG_ACTIVE_LEVEL
#endif  // SPDLOG_ACTIVE_LEVEL

// #define SPDLOG_ACTIVE_LEVEL 0 /* trace */
#define SPDLOG_ACTIVE_LEVEL 1 /* debug */
// #define SPDLOG_ACTIVE_LEVEL 2 /* info */
// #define SPDLOG_ACTIVE_LEVEL 3 /* warn */
// #define SPDLOG_ACTIVE_LEVEL 4 /* error */
// #define SPDLOG_ACTIVE_LEVEL 5 /* critical */
// #define SPDLOG_ACTIVE_LEVEL 6 /* off */

#include <spdlog/spdlog.h>
using logger_t = std::shared_ptr<spdlog::logger>;

namespace uinta {
void initSpdlog();
}

#endif  // !UINTA_LOGGING_HPP
