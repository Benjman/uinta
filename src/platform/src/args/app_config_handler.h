#ifndef SRC_PLATFORM_SRC_ARGS_APP_CONFIG_HANDLER_H_
#define SRC_PLATFORM_SRC_ARGS_APP_CONFIG_HANDLER_H_

#include <array>
#include <string>

#include "uinta/args.h"

namespace uinta {

struct AppConfigHandler : ArgHandler {
  static constexpr std::array<std::string, 2> Keys = {"-o", "--opt"};

  static constexpr std::string PrmaryKey = Keys[1];

  explicit AppConfigHandler(ArgsProcessor* processor) noexcept
      : ArgHandler(processor) {}

  [[nodiscard]] ArgsProcessor::Arg getArg() const noexcept override {
    return ArgsProcessor::AppConfigFilePath;
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

#endif  // SRC_PLATFORM_SRC_ARGS_APP_CONFIG_HANDLER_H_
