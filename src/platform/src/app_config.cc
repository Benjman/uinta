#include "uinta/app_config.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "uinta/args.h"

namespace uinta {

AppConfig::AppConfig(const std::filesystem::path& filePath, const ArgsProcessor* args) noexcept {
  // Allow command-line override via --opt=<path>
  if (auto filePathOpt = args->getValue(ArgsProcessor::AppConfigFilePath); filePathOpt.has_value()) {
    if (!filePathOpt->empty() && !std::all_of(filePathOpt.value().begin(), filePathOpt.value().end(), isspace)) {
      filePath_ = filePathOpt.value();
      return;
    }
  }

  if (!std::filesystem::exists(filePath)) {
    LOG(WARNING) << absl::StrFormat("AppConfig path '%s' not found; creating a default config.", filePath.string());

    std::error_code ec;
    if (auto parent = filePath.parent_path(); !parent.empty()) {
      std::filesystem::create_directories(parent, ec);
    }
    if (!ec) {
      if (std::ofstream out(filePath); out) {
        out << absl::StrFormat("schemaVersion: %d\n", CurrentSchemaVersion);
      }
    }
    if (ec || !std::filesystem::exists(filePath)) {
      LOG(FATAL) << absl::StrFormat("Failed to create default AppConfig at '%s': %s", filePath.string(), ec.message());
    }
  }

  filePath_ = filePath.string();
}

}  // namespace uinta
