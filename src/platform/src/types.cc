#include "uinta/types.h"

#include "absl/log/log.h"

namespace uinta {

void logError(const char* message) noexcept { LOG(ERROR) << message; }
void logFatal(const char* message) noexcept { LOG(FATAL) << message; }
void logInfo(const char* message) noexcept { LOG(INFO) << message; }
void logWarning(const char* message) noexcept { LOG(WARNING) << message; }

}  // namespace uinta
