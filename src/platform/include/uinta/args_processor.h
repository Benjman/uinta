#ifndef SRC_PLATFORM_INCLUDE_UINTA_ARGS_PROCESSOR_H_
#define SRC_PLATFORM_INCLUDE_UINTA_ARGS_PROCESSOR_H_

#include "uinta/types.h"

namespace uinta {

// TODO: WIP
class ArgsProcessor {
 public:
  ArgsProcessor(i32 argc, const char** argv) noexcept
      : count_(argc), values_(argv) {}

  i32 count() const noexcept { return count_; }

  const char** values() const noexcept { return values_; }

 private:
  i32 count_;
  const char** values_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_ARGS_PROCESSOR_H_
