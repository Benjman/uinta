#include <uinta/utils/string_utils.h>

#include <uinta/logging.hpp>
#include <uinta/runner/runner.hpp>

namespace uinta {

struct RunnerArg {
  std::string key = "";
  std::string value = "";

  explicit RunnerArg(const std::string& key) : key(key) {
  }

  RunnerArg(const RunnerArg& other) {
    *this = other;
  }

  RunnerArg& operator=(const RunnerArg& rhs) {
    key = rhs.key;
    value = rhs.value;
    return *this;
  }
};

void expectNoValue(const RunnerArg& arg);
std::vector<RunnerArg> extractArgs(int argc, const char** argv);

bool processArg_noGrid(Runner* runner, const RunnerArg& arg);
bool processArg_noRendering(Runner* runner, const RunnerArg& arg);

void processArgs(Runner* runner, int argc, const char** argv) {
  for (auto& arg : extractArgs(argc, argv)) {
    if (processArg_noGrid(runner, arg)) continue;
    if (processArg_noRendering(runner, arg)) continue;
    SPDLOG_WARN("Unhandled argument: '{}'", arg.key);
  }
}

std::vector<RunnerArg> extractArgs(int argc, const char** argv) {
  std::vector<RunnerArg> result;
  RunnerArg* current = nullptr;
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] == '-') {
      std::string key = argv[i];
      current = &result.emplace_back(key);
      if (auto eqPos = key.find('='); eqPos != std::string::npos) {
        current->key = key.substr(0, eqPos);
        current->value = key.substr(eqPos + 1, key.length());
      }
    } else if (current) {
      current->value += argv[i];
    }
  }
  return result;
}

bool processArg_noGrid(Runner* runner, const RunnerArg& arg) {
  const char* keys[] = {
      "--no-grid",
      nullptr,
  };
  if (!containsKey(arg.key.c_str(), keys)) return false;
  setFlag(RUNNER_FLAG_GRID, false, runner->flags);
  expectNoValue(arg);
  SPDLOG_INFO("Cartesian grid disabled via argument '{}'.", arg.key);
  return true;
}

bool processArg_noRendering(Runner* runner, const RunnerArg& arg) {
  const char* keys[] = {
      "--no-render",
      nullptr,
  };
  if (!containsKey(arg.key.c_str(), keys)) return false;
  setFlag(RUNNER_FLAG_RENDERING, false, runner->flags);
  expectNoValue(arg);
  SPDLOG_INFO("Rendering disabled via argument '{}'.", arg.key);
  return true;
}

void expectNoValue(const RunnerArg& arg) {
  if (!arg.value.empty()) SPDLOG_WARN("Argument '{}' provided an unexpected value.", arg.key);
}

}  // namespace uinta
