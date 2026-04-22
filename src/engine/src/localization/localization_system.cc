#include "uinta/localization/localization_system.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>
#include <yaml-cpp/yaml.h>

#include <cassert>
#include <filesystem>
#include <string>

#include "uinta/file.h"

namespace uinta {

LocalizationSystem::LocalizationSystem(Locale locale) noexcept
    : locale_(locale) {
  if (locale == Locale::Unknown) {
    LOG(FATAL) << "Unknown locale";
  }

  auto relPath = absl::StrFormat("localization/%s.yml", toString(locale_));
  auto resolved = FindFile(relPath);

  assert(resolved.has_value() &&
         "LocalizationSystem: failed to locate localization resource file.");

  if (!resolved.has_value()) {
    LOG(FATAL) << absl::StrFormat(
        "LocalizationSystem: failed to locate localization resource '%s'.",
        relPath);
  }

  std::string content;
  if (auto status = ReadFile(*resolved, content); !status.ok()) {
    LOG(FATAL) << absl::StrFormat(
        "LocalizationSystem: failed to read localization resource '%s': %s",
        resolved->string(), status.message());
  }

  try {
    YAML::Node root = YAML::Load(content);
    if (!root.IsMap()) {
      LOG(FATAL) << absl::StrFormat(
          "LocalizationSystem: expected top-level map in '%s'.",
          resolved->string());
    }
    for (const auto& entry : root) {
      if (!entry.first.IsScalar() || !entry.second.IsScalar()) {
        LOG(WARNING) << absl::StrFormat(
            "LocalizationSystem: skipping non-scalar entry in '%s'.",
            resolved->string());
        continue;
      }
      const auto yamlKey = entry.first.as<std::string>();
      const auto localizationKey = fromKey(yamlKey);
      if (!localizationKey.has_value()) {
        LOG(WARNING) << absl::StrFormat(
            "LocalizationSystem: unknown key '%s' in '%s'.", yamlKey,
            resolved->string());
        continue;
      }
      strings_.emplace(*localizationKey, entry.second.as<std::string>());
    }
  } catch (const YAML::Exception& e) {
    LOG(FATAL) << absl::StrFormat(
        "LocalizationSystem: failed to parse localization resource '%s': %s",
        resolved->string(), e.what());
  }

  LOG(INFO) << absl::StrFormat(
      "LocalizationSystem: loaded %d strings for locale '%s' from '%s'.",
      strings_.size(), toString(locale_), resolved->string());
}

std::string LocalizationSystem::getString(Localization key) const noexcept {
  if (auto it = strings_.find(key); it != strings_.end()) {
    return it->second;
  }
  const std::string_view yamlKey = toKey(key);
  LOG(WARNING) << absl::StrFormat(
      "LocalizationSystem: missing key '%s' for locale '%s'.", yamlKey,
      toString(locale_));
  return std::string(yamlKey);
}

}  // namespace uinta
