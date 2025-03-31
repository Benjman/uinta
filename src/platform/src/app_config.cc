#include "uinta/app_config.h"

#include <optional>
#include <string>

#include "absl/log/log.h"
#include "uinta/args.h"
#include "uinta/cfg.h"
#include "uinta/file.h"

namespace uinta {

AppConfig::AppConfig(std::string filename, const ArgsProcessor* args,
                     FileSystem* fs) noexcept
    : fs_(fs) {
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
      [&](std::string filename,
          const FileSystem* fs) noexcept -> std::optional<std::string> {
    for (auto& dir : FileSystem::SearchDirs()) {
      auto path = absl::StrFormat("%s/%s", dir, filename);
      if (fs->exists(path)) {
        return path;
      }
    }
    return std::nullopt;
  };

  auto tryDefault =
      [&](std::string filename,
          const FileSystem* fs) noexcept -> std::optional<std::string> {
    auto path = absl::StrFormat("%s/%s", cfg::DefaultAppConfigDest, filename);
    if (fs->exists(path)) {
      return path;
    }
    return std::nullopt;
  };

  if (auto opt = tryArgs(args); opt.has_value()) {
    filePath_ = opt.value();
  } else if (auto opt = tryDefault(filename, fs_); opt.has_value()) {
    filePath_ = opt.value();
  } else if (auto opt = trySearchDirs(filename, fs_); opt.has_value()) {
    filePath_ = opt.value();
  } else {
    // User config file does not exist. Let's create one:
    filePath_ = absl::StrFormat("%s/%s", cfg::DefaultAppConfigDest, filename);
    if (fs_->exists(filePath_)) {
      LOG(FATAL) << absl::StrFormat(
          "Duplicate file detected when attempting to write %s: %s", filename,
          filePath_);
    } else if (auto status = fs->copy(cfg::DefaultAppConfigPath, filePath_);
               status.ok()) {
      LOG(INFO) << absl::StrFormat("Created config file %s", filePath_);
    } else {
      status_ = InternalError(
          absl::StrFormat("Failed to write options file '%s'", filePath_));
    }
  }
}

}  // namespace uinta
