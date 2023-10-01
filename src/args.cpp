#include <uinta/types.h>

#include <uinta/logging.hpp>
#include <uinta/runner.hpp>

namespace uinta {

using RunnerArg = std::pair<std::string, std::string>;

void expectNoValue(const RunnerArg& arg);
std::vector<RunnerArg> extractArgs(i32 argc, const char** argv);

bool processArg_width(Runner* runner, const RunnerArg& arg);
bool processArg_height(Runner* runner, const RunnerArg& arg);
bool containsKey(const char* const key, const char** const keys);

void processArgs(Runner* runner, i32 argc, const char** argv) {
  for (auto& arg : extractArgs(argc, argv)) {
    if (processArg_height(runner, arg)) continue;
    if (processArg_width(runner, arg)) continue;
    SPDLOG_WARN("Unhandled argument: '{}'", arg.first);
  }
}

std::vector<RunnerArg> extractArgs(i32 argc, const char** argv) {
  std::vector<RunnerArg> result;
  RunnerArg current;
  for (i32 i = 0; i < argc; i++) {
    if (i % 2 == 0) current = {};
    if (argv[i][0] == '-') {
      current.first = argv[i];
      if (auto eqPos = current.first.find('='); eqPos != std::string::npos) {
        current.second = current.first.substr(eqPos + 1, current.first.length());
        current.first = current.first.substr(0, eqPos);
      }
      result.emplace_back(current);
    } else {
      current.second += argv[i];
    }
  }
  return result;
}

bool processArg_height(Runner* runner, const RunnerArg& arg) {
  const char* keys[] = {
      "--height",
      "-h",
      nullptr,
  };
  if (!containsKey(arg.first.c_str(), keys)) return false;
  runner->handleWindowSizeChanged(runner->window().width, std::stof(arg.second));
  SPDLOG_INFO("`{}`: Height set to: {}", arg.first, arg.second);
  return true;
}

bool processArg_width(Runner* runner, const RunnerArg& arg) {
  const char* keys[] = {
      "--width",
      "-w",
      nullptr,
  };
  if (!containsKey(arg.first.c_str(), keys)) return false;
  runner->handleWindowSizeChanged(std::stof(arg.second), runner->window().height);
  SPDLOG_INFO("`{}`: Width set to: {}", arg.first, arg.second);
  return true;
}

void expectNoValue(const RunnerArg& arg) {
  if (!arg.second.empty()) SPDLOG_WARN("Argument '{}' provided an unexpected value.", arg.first);
}

bool containsKey(const char* const key, const char** const keys) {
  for (int i = 0; keys[i]; i++) {
    if (strcmp(key, keys[i]) == 0) return true;
#ifdef UINTA_DEBUG
    if (i >= 100 && i % 100 == 0) SPDLOG_WARN("`containsKey()` has looped {} times without encountering a nullptr.", i);
#endif
  }
  return false;
}

}  // namespace uinta
