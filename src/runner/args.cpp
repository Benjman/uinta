#include <uinta/utils/string_utils.h>

#include <string>
#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>
#include <vector>

namespace uinta {

using RunnerArg = std::pair<std::string, std::string>;

void expectNoValue(const RunnerArg& arg);
std::vector<RunnerArg> extractArgs(i32 argc, const char** argv);

bool processArg_noGrid(Runner* runner, const RunnerArg& arg);
bool processArg_noRendering(Runner* runner, const RunnerArg& arg);
bool processArg_width(Runner* runner, const RunnerArg& arg);
bool processArg_height(Runner* runner, const RunnerArg& arg);

void processArgs(Runner* runner, i32 argc, const char** argv) {
  for (auto& arg : extractArgs(argc, argv)) {
    if (processArg_noGrid(runner, arg)) continue;
    if (processArg_noRendering(runner, arg)) continue;
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

bool processArg_noGrid(Runner* runner, const RunnerArg& arg) {
  const char* keys[] = {
      "--no-grid",
      nullptr,
  };
  if (!containsKey(arg.first.c_str(), keys)) return false;
  runner->scene().flag(Scene::GRID_ENABLED, false);
  expectNoValue(arg);
  SPDLOG_INFO("Cartesian grid disabled via argument '{}'.", arg.first);
  return true;
}

bool processArg_noRendering(Runner* runner, const RunnerArg& arg) {
  const char* keys[] = {
      "--no-render",
      nullptr,
  };
  if (!containsKey(arg.first.c_str(), keys)) return false;
  runner->flag(Runner::RENDERING_ENABLED, false);
  expectNoValue(arg);
  SPDLOG_INFO("Rendering disabled via argument '{}'.", arg.first);
  return true;
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

}  // namespace uinta
