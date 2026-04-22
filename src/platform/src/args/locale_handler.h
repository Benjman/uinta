#ifndef SRC_PLATFORM_SRC_ARGS_LOCALE_HANDLER_H_
#define SRC_PLATFORM_SRC_ARGS_LOCALE_HANDLER_H_

#include <array>
#include <string>

#include "uinta/args.h"

namespace uinta {

// CLI handler for `--locale=<value>`. The raw string value is stored; the
// engine resolves it to a `Locale` enum via `toLocale` at consumption time.
//
// NOTE: This handler lives in platform only because `ArgsProcessor` currently
// requires handlers to be registered inside its own constructor. See
// [PM#36] for the follow-up work to let engine-layer services register their
// own handlers.
struct LocaleHandler : ArgHandler {
  static constexpr std::array<std::string, 1> Keys = {"--locale"};

  static constexpr std::string PrimaryKey = Keys[0];

  explicit LocaleHandler(ArgsProcessor* processor) noexcept
      : ArgHandler(processor) {}

  [[nodiscard]] ArgsProcessor::Arg getArg() const noexcept override {
    return ArgsProcessor::Locale;
  }

  [[nodiscard]] constexpr std::span<const std::string> getKeys()
      const noexcept override {
    return Keys;
  }

  [[nodiscard]] constexpr std::string getPrimaryKey() const noexcept override {
    return PrimaryKey;
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_SRC_ARGS_LOCALE_HANDLER_H_
