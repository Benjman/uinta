#ifndef SRC_PLATFORM_SRC_ARGS_HELP_HANDLER_H_
#define SRC_PLATFORM_SRC_ARGS_HELP_HANDLER_H_

#include <algorithm>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "app_config_handler.h"
#include "uinta/args.h"
#include "uinta/status.h"

namespace uinta {

struct HelpHandler : ArgHandler {
  static constexpr std::array<std::string, 2> Keys =
      std::array<std::string, 2>{"-h", "--help"};

  static constexpr std::string PrimaryKey = Keys[1];

  explicit HelpHandler(ArgsProcessor* processor) noexcept
      : ArgHandler(processor) {}

  ArgsProcessor::Arg getArg() const noexcept override {
    return ArgsProcessor::Help;
  }

  constexpr std::span<const std::string> getKeys() const noexcept override {
    return Keys;
  }

  std::string getPrmaryKey() const noexcept override { return PrimaryKey; }

  Status handle(std::string) noexcept override {
    std::vector<std::span<const std::string>> collections = {
        AppConfigHandler::Keys,
        HelpHandler::Keys,
    };
    size_t colWidth = 0;
    for (const auto& collection : collections) {
      for (const auto& key : collection) {
        colWidth = std::max(colWidth, key.size());
      }
    }
    constexpr size_t padding = 2;
    colWidth += padding;
    std::string output = "Uinta Engine options:";
    for (const auto& collection : collections) {
      output = absl::StrCat(output, "\n\t");
      for (const auto& key : collection) {
        output = absl::StrCat(output, absl::StrFormat("%-*s", colWidth, key));
      }
    }
    return CancelledError(output);
  };
};

}  // namespace uinta

#endif  // SRC_PLATFORM_SRC_ARGS_HELP_HANDLER_H_
