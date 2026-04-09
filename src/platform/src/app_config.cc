#include "uinta/app_config.h"

#include <filesystem>
#include <string>

#include "uinta/args.h"

namespace uinta {

AppConfig::AppConfig(const std::filesystem::path& filePath,
                     const ArgsProcessor* args) noexcept {
  // Allow command-line override via --opt=<path>
  if (auto filePathOpt = args->getValue(ArgsProcessor::AppConfigFilePath);
      filePathOpt.has_value()) {
    if (!filePathOpt->empty() &&
        !std::all_of(filePathOpt.value().begin(), filePathOpt.value().end(),
                     isspace)) {
      filePath_ = filePathOpt.value();
      return;
    }
  }

  filePath_ = filePath.string();
}

}  // namespace uinta
