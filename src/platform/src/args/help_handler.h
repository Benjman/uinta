#ifndef SRC_PLATFORM_SRC_ARGS_HELP_HANDLER_H_
#define SRC_PLATFORM_SRC_ARGS_HELP_HANDLER_H_

#include <absl/strings/str_cat.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "./app_config_handler.h"
#include "./locale_handler.h"
#include "./plugin_path_handler.h"
#include "uinta/args.h"
#include "uinta/status.h"

namespace uinta {

struct HelpHandler : ArgHandler {
  static constexpr std::array<std::string, 2> Keys =
      std::array<std::string, 2>{"-h", "--help"};

  static constexpr std::string PrimaryKey = Keys[1];

  explicit HelpHandler(ArgsProcessor* processor) noexcept
      : ArgHandler(processor) {}

  [[nodiscard]] ArgsProcessor::Arg getArg() const noexcept override {
    return ArgsProcessor::Help;
  }

  [[nodiscard]] constexpr std::span<const std::string> getKeys()
      const noexcept override {
    return Keys;
  }

  [[nodiscard]] std::string getPrimaryKey() const noexcept override {
    return PrimaryKey;
  }

  [[nodiscard]] Status handle(const std::string& /*value*/) noexcept override {
    std::vector<std::span<const std::string>> collections = {
        AppConfigHandler::Keys,
        HelpHandler::Keys,
        LocaleHandler::Keys,
        PluginPathHandler::Keys,
    };
    size_t colWidth = 0;
    for (const auto& collection : collections) {
      for (const auto& key : collection) {
        colWidth = std::max(colWidth, key.size());
      }
    }
    constexpr size_t padding = 2;
    colWidth += padding;
    // TODO: localize
    printf("Uinta Engine help:");
    for (const auto& collection : collections) {
      std::string keys;
      for (const auto& key : collection) {
        keys = absl::StrCat(keys, key, ", ");
      }
      std::printf("\n\t%s", keys.data());
    }
    std::printf("\n");
    return absl::AbortedError("");
  };
};

}  // namespace uinta

#endif  // SRC_PLATFORM_SRC_ARGS_HELP_HANDLER_H_
