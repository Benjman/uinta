#ifndef SRC_PLATFORM_SRC_ARGS_PLUGIN_PATH_HANDLER_H_
#define SRC_PLATFORM_SRC_ARGS_PLUGIN_PATH_HANDLER_H_

#include <array>
#include <string>

#include "uinta/args.h"

namespace uinta {

struct PluginPathHandler : ArgHandler {
  static constexpr std::array<std::string, 1> Keys = {"--plugin-path"};

  static constexpr std::string PrmaryKey = Keys[0];

  explicit PluginPathHandler(ArgsProcessor* processor) noexcept
      : ArgHandler(processor) {}

  [[nodiscard]] ArgsProcessor::Arg getArg() const noexcept override {
    return ArgsProcessor::PluginPath;
  }

  [[nodiscard]] constexpr std::span<const std::string> getKeys()
      const noexcept override {
    return Keys;
  }

  [[nodiscard]] constexpr std::string getPrimaryKey() const noexcept override {
    return PrmaryKey;
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_SRC_ARGS_PLUGIN_PATH_HANDLER_H_
