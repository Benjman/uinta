#ifndef UINTA_LOGGING_HPP
#define UINTA_LOGGING_HPP

// TODO Implement preprocessor logging: https://trello.com/c/WAGDemX6/7-preprocessor-logging

#include <spdlog/common.h>
#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/stopwatch.h>

using logger_t = std::shared_ptr<spdlog::logger>;

#endif // !UINTA_LOGGING_HPP
