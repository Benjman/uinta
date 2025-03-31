#include "uinta/app_config_yaml.h"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "uinta/args.h"
#include "uinta/file.h"

namespace uinta {

constexpr auto Separator = ' ';

AppConfigYamlImpl::AppConfigYamlImpl(const ArgsProcessor* args) noexcept
    : AppConfig("config.yml", args) {
  if (!status().ok()) {
    return;
  }

  std::error_code ec;
  if (!std::filesystem::exists(filePath(), ec)) {
    status(InternalError(absl::StrFormat(
        "Failed to initialize AppConfig path '%s'.", filePath())));
    return;
  }

  std::string content;
  if (auto readStatus = ReadFile(filePath(), content); !readStatus.ok()) {
    status(readStatus);
    return;
  }

  rootNode_ = YAML::Load(content);

  // Read schema version from top level (0 = legacy/absent)
  if (rootNode_["schemaVersion"] && rootNode_["schemaVersion"].IsScalar()) {
    schemaVersion_ = rootNode_["schemaVersion"].as<i32>();
  } else {
    schemaVersion_ = 0;
  }
}

std::optional<YAML::Node> findByKey(const YAML::Node* rootNode,
                                    std::string key) noexcept {
  std::vector<std::string> keys = absl::StrSplit(key, '.');
  auto node = *rootNode;
  if (keys.size()) {
    for (size_t i = 0; i < keys.size(); i++) {
      auto key = keys.at(i);

      // NOTE: `YAML::Node::reset(...)` called to ensure the root node remains
      // unchanged. See https://github.com/jbeder/yaml-cpp/issues/208
      node.reset(node[key]);
    }
    if (node.IsDefined()) {
      return node;
    }
  }
  return std::nullopt;
}

template <typename T>
std::optional<T> getVal(const YAML::Node* rootNode, std::string key) noexcept {
  if (auto opt = findByKey(rootNode, key); opt.has_value()) {
    if (auto node = opt.value(); node.IsDefined()) {
      T value = node.as<T>();
      return value;
    }
  }
  LOG(WARNING) << absl::StrFormat("AppConfig not found for key '%s'.", key);
  return std::nullopt;
}

std::optional<std::string> AppConfigYamlImpl::getString(
    std::string key) const noexcept {
  return getVal<std::string>(&rootNode_, key);
}

std::optional<bool> AppConfigYamlImpl::getBoolean(
    std::string key) const noexcept {
  return getVal<bool>(&rootNode_, key);
}

std::optional<i32> AppConfigYamlImpl::getInt(std::string key) const noexcept {
  return getVal<i32>(&rootNode_, key);
}

std::optional<i32*> AppConfigYamlImpl::getInt2(std::string key,
                                               i32* ptr) const noexcept {
  if (auto opt = getString(key); opt.has_value()) {
    std::vector<std::string> value = absl::StrSplit(opt.value(), " ");
    ptr[0] = std::stoi(value.at(0));
    ptr[1] = std::stoi(value.at(1));
    return ptr;
  }
  return std::nullopt;
}

std::optional<i32*> AppConfigYamlImpl::getInt3(std::string key,
                                               i32* ptr) const noexcept {
  if (auto opt = getString(key); opt.has_value()) {
    std::vector<std::string> value = absl::StrSplit(opt.value(), " ");
    ptr[0] = std::stoi(value.at(0));
    ptr[1] = std::stoi(value.at(1));
    ptr[2] = std::stoi(value.at(2));
    return ptr;
  }
  return std::nullopt;
}

std::optional<i32*> AppConfigYamlImpl::getInt4(std::string key,
                                               i32* ptr) const noexcept {
  if (auto opt = getString(key); opt.has_value()) {
    std::vector<std::string> value = absl::StrSplit(opt.value(), " ");
    ptr[0] = std::stoi(value.at(0));
    ptr[1] = std::stoi(value.at(1));
    ptr[2] = std::stoi(value.at(2));
    ptr[3] = std::stoi(value.at(3));
    return ptr;
  }
  return std::nullopt;
}

std::optional<f32> AppConfigYamlImpl::getFloat(std::string key) const noexcept {
  return getVal<f32>(&rootNode_, key);
}

std::optional<f32*> AppConfigYamlImpl::getFloat2(std::string key,
                                                 f32* ptr) const noexcept {
  if (auto opt = getString(key); opt.has_value()) {
    std::vector<std::string> value = absl::StrSplit(opt.value(), " ");
    ptr[0] = std::stof(value.at(0));
    ptr[1] = std::stof(value.at(1));
    return ptr;
  }
  return std::nullopt;
}

std::optional<f32*> AppConfigYamlImpl::getFloat3(std::string key,
                                                 f32* ptr) const noexcept {
  if (auto opt = getString(key); opt.has_value()) {
    std::vector<std::string> value = absl::StrSplit(opt.value(), " ");
    ptr[0] = std::stof(value.at(0));
    ptr[1] = std::stof(value.at(1));
    ptr[2] = std::stof(value.at(2));
    return ptr;
  }
  return std::nullopt;
}

std::optional<f32*> AppConfigYamlImpl::getFloat4(std::string key,
                                                 f32* ptr) const noexcept {
  if (auto opt = getString(key); opt.has_value()) {
    std::vector<std::string> value = absl::StrSplit(opt.value(), " ");
    ptr[0] = std::stof(value.at(0));
    ptr[1] = std::stof(value.at(1));
    ptr[2] = std::stof(value.at(2));
    ptr[3] = std::stof(value.at(3));
    return ptr;
  }
  return std::nullopt;
}

Status setNodeValue(YAML::Node* rootNode_, std::string key,
                    std::string value) noexcept {
  if (auto opt = findByKey(rootNode_, key); opt.has_value()) {
    opt.value() = value;
    return OkStatus();
  } else {
    return InternalError(absl::StrFormat("Key not found: %s", key));
  }
}

Status setNodeValue(YAML::Node* rootNode_, std::string key,
                    i32 value) noexcept {
  if (auto opt = findByKey(rootNode_, key); opt.has_value()) {
    opt.value() = value;
    return OkStatus();
  } else {
    return InternalError(absl::StrFormat("Key not found: %s", key));
  }
}

Status setNodeValue(YAML::Node* rootNode_, std::string key,
                    f32 value) noexcept {
  if (auto opt = findByKey(rootNode_, key); opt.has_value()) {
    opt.value() = value;
    return OkStatus();
  } else {
    return InternalError(absl::StrFormat("Key not found: %s", key));
  }
}

Status AppConfigYamlImpl::setString(std::string key,
                                    std::string value) noexcept {
  return setNodeValue(&rootNode_, key, value);
}

Status AppConfigYamlImpl::setBoolean(std::string key, bool value) noexcept {
  return setNodeValue(&rootNode_, key, value ? "true" : "false");
}

Status AppConfigYamlImpl::setInt(std::string key, i32 value) noexcept {
  return setNodeValue(&rootNode_, key, value);
}

Status AppConfigYamlImpl::setInt2(std::string key, i32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 2; i++) {
    v += absl::StrFormat("%i", value[i]);
    if (i != 1) v += Separator;
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setInt3(std::string key, i32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 3; i++) {
    v += absl::StrFormat("%i", value[i]);
    if (i != 2) v += Separator;
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setInt4(std::string key, i32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 4; i++) {
    v += absl::StrFormat("%i", value[i]);
    if (i != 3) v += Separator;
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setFloat(std::string key, f32 value) noexcept {
  return setNodeValue(&rootNode_, key, value);
}

Status AppConfigYamlImpl::setFloat2(std::string key, f32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 2; i++) {
    v += absl::StrFormat("%f", value[i]);
    if (i != 1) v += Separator;
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setFloat3(std::string key, f32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 3; i++) {
    v += absl::StrFormat("%f", value[i]);
    if (i != 2) v += Separator;
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setFloat4(std::string key, f32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 4; i++) {
    v += absl::StrFormat("%f", value[i]);
    if (i != 3) v += Separator;
  }
  return setNodeValue(&rootNode_, key, v);
}

void AppConfigYamlImpl::flush() const noexcept {
  std::ofstream fout(filePath());
  fout << rootNode_;
}

}  // namespace uinta
