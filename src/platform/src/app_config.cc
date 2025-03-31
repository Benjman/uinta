#include "uinta/app_config.h"

#include <filesystem>
#include <optional>
#include <string>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "uinta/args.h"
#include "uinta/cfg.h"
#include "uinta/file.h"

namespace uinta {

AppConfig::AppConfig(std::string filename, const ArgsProcessor* args) noexcept {
  auto tryArgs =
      [&](const ArgsProcessor* args) noexcept -> std::optional<std::string> {
    if (auto filePathOpt = args->getValue(ArgsProcessor::AppConfigFilePath);
        filePathOpt.has_value()) {
      if (!filePathOpt->empty() &&
          // Check if input is not all whitespace:
          !std::all_of(filePathOpt.value().begin(), filePathOpt.value().end(),
                       isspace)) {
        return filePathOpt.value();
      }
    }
    return std::nullopt;
  };

  auto trySearchDirs =
      [&](const std::string& filename) noexcept -> std::optional<std::string> {
    for (const auto& dir : FileSearchDirs()) {
      auto path = std::filesystem::path(dir) / filename;
      std::error_code ec;
      if (std::filesystem::exists(path, ec)) {
        return path.string();
      }
    }
    return std::nullopt;
  };

  auto tryDefault =
      [&](const std::string& filename) noexcept -> std::optional<std::string> {
    auto path =
        std::filesystem::path(cfg::DefaultAppConfigDest) / filename;
    std::error_code ec;
    if (std::filesystem::exists(path, ec)) {
      return path.string();
    }
    return std::nullopt;
  };

  if (auto opt = tryArgs(args); opt.has_value()) {
    filePath_ = opt.value();
  } else if (auto opt = tryDefault(filename); opt.has_value()) {
    filePath_ = opt.value();
  } else if (auto opt = trySearchDirs(filename); opt.has_value()) {
    filePath_ = opt.value();
  } else {
    // User config file does not exist. Let's create one:
    filePath_ =
        (std::filesystem::path(cfg::DefaultAppConfigDest) / filename).string();
    
    std::error_code ec;
    if (std::filesystem::exists(filePath_, ec)) {
      LOG(FATAL) << absl::StrFormat(
          "Duplicate file detected when attempting to write %s: %s", filename,
          filePath_);
    } else if (auto status = CopyFile(cfg::DefaultAppConfigPath, filePath_);
               status.ok()) {
      LOG(INFO) << absl::StrFormat("Created config file %s", filePath_);
    } else {
      status_ = InternalError(
          absl::StrFormat("Failed to write options file '%s': %s", filePath_,
                          status.message()));
    }
  }
}

}  // namespace uinta
