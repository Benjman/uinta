#include "uinta/args.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "./args/app_config_handler.h"
#include "./args/help_handler.h"

namespace uinta {

ArgsProcessor::ArgsProcessor(i32 argc, const char** argv) noexcept {
  using KeyValuePair = std::pair<std::string, std::string>;
  constexpr const auto kvp = [&](std::string arg) -> KeyValuePair {
    constexpr const char* defaultValue = "";
    constexpr const char seperator = '=';
    KeyValuePair result(arg, defaultValue);
    size_t idx = arg.length();
    result.first = arg.substr(0, idx);
    if (idx = arg.find(seperator); idx != std::string::npos) {
      result.first = arg.substr(0, idx);
      result.second = arg.substr(idx + 1, arg.size() - idx);
    }
    return result;
  };

  std::vector<std::unique_ptr<ArgHandler>> handlers;
  handlers.push_back(std::make_unique<AppConfigHandler>(this));
  handlers.push_back(std::make_unique<HelpHandler>(this));

  for (auto i = 0; i < argc; i++) {
    auto valuePair = kvp(argv[i]);
    for (auto& handler : handlers) {
      if (handler->containsKey(valuePair.first)) {
        if (store_.contains(handler->getArg())) {
          // TODO: Warning??
        }
        if (status_ = handler->handle(valuePair.second); !status_.ok()) {
          return;
        }
        continue;
      }
    }
  }
}

std::optional<ArgsProcessor::StoreValue> ArgsProcessor::getValue(
    Arg arg) const noexcept {
  if (auto itr = store_.find(arg); itr == store_.end()) {
    return std::nullopt;
  }
  return store_.at(arg);
}

std::optional<bool> ArgsProcessor::getBoolean(Arg arg) const noexcept {
  if (auto itr = store_.find(arg); itr == store_.end()) {
    return std::nullopt;
  } else if (itr->second != ArgsProcessor::True &&
             itr->second != ArgsProcessor::False) {
    return std::nullopt;
  } else {
    return itr->second == ArgsProcessor::True;
  }
}

std::optional<i32> ArgsProcessor::getInt(Arg arg) const noexcept {
  if (auto itr = store_.find(arg); itr == store_.end()) {
    return std::nullopt;
  } else {
    try {
      return std::stoi(itr->second);
    } catch (...) {
      // TODO: Warning?
      return std::nullopt;
    }
  }
}

}  // namespace uinta
