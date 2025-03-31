#ifndef SRC_PLATFORM_INCLUDE_UINTA_ARGS_H_
#define SRC_PLATFORM_INCLUDE_UINTA_ARGS_H_

#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>

#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

class ArgsProcessor {
  friend struct ArgHandler;

 public:
  enum Arg : u16 {
    Help,
  };

  using StoreValue = std::string;
  using Store = std::unordered_map<Arg, StoreValue>;

  static constexpr StoreValue True = "t";
  static constexpr StoreValue False = "f";

  ArgsProcessor(i32, const char**) noexcept;

  Status status() const noexcept { return status_; }

  std::optional<StoreValue> getValue(Arg) const noexcept;

  std::optional<bool> getBoolean(Arg) const noexcept;

  std::optional<i32> getInt(Arg) const noexcept;

 private:
  Status status_ = OkStatus();
  Store store_;
};

struct ArgHandler;
class ArgsProcessor;

struct ArgHandler {
  explicit ArgHandler(ArgsProcessor* processor) noexcept
      : processor_(processor) {}

  virtual ArgsProcessor::Arg getArg() const noexcept = 0;

  virtual constexpr std::span<const std::string> getKeys() const noexcept = 0;

  virtual constexpr std::string getPrmaryKey() const noexcept = 0;

  virtual Status handle(std::string value) noexcept {
    return setValue(this, value);
  }

  bool containsKey(const std::string key) const noexcept {
    auto end = getKeys().end();
    return std::find(getKeys().begin(), end, key) != end;
  }

 protected:
  ArgsProcessor* processor_;

  static Status setValue(ArgHandler* handler,
                         ArgsProcessor::StoreValue value) noexcept {
    auto status = OkStatus();
    if (handler->processor_->store_.contains(handler->getArg())) {
      auto status = AlreadyExistsError(
          absl::StrFormat("%s alrady exists", handler->getPrmaryKey()));
    }
    handler->processor_->store_.insert_or_assign(handler->getArg(), value);
    return status;
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_ARGS_H_
