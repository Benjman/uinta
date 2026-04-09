#include "uinta/app_config_yaml.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>
#include <absl/strings/str_split.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "uinta/args.h"
#include "uinta/dirs.h"
#include "uinta/file.h"
#include "uinta/status.h"

namespace uinta {

constexpr auto Separator = ' ';

AppConfigYamlImpl::AppConfigYamlImpl(const ArgsProcessor* args) noexcept
    : AppConfig(GetDir<DirType::DataDir>().value_or("") / "uinta/config.yml",
                args) {
  if (!status().ok()) {
    return;
  }

  std::error_code ec;
  if (!std::filesystem::exists(filePath(), ec)) {
    status(NotFoundError(
        absl::StrFormat("Failed to locate AppConfig path '%s'.", filePath())));
    LOG(WARNING) << status().message();
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
                                    const std::string& key) noexcept {
  std::vector<std::string> keys = absl::StrSplit(key, '.');
  auto node = *rootNode;
  if (keys.size() != 0u) {
    for (const auto& key : keys) {
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
std::optional<T> getVal(const YAML::Node* rootNode,
                        const std::string& key) noexcept {
  if (auto opt = findByKey(rootNode, key); opt.has_value()) {
    if (const auto& node = opt.value(); node.IsDefined()) {
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

YAML::Node findOrCreateByKey(YAML::Node* rootNode,
                             const std::string& key) noexcept {
  std::vector<std::string> keys = absl::StrSplit(key, '.');
  auto node = *rootNode;
  for (const auto& k : keys) {
    if (!node[k].IsDefined()) {
      node[k] = YAML::Node(YAML::NodeType::Map);
    }
    node.reset(node[k]);
  }
  return node;
}

Status setNodeValue(YAML::Node* rootNode_, const std::string& key,
                    const std::string& value) noexcept {
  auto node = findOrCreateByKey(rootNode_, key);
  node = value;
  return OkStatus();
}

Status setNodeValue(YAML::Node* rootNode_, const std::string& key,
                    i32 value) noexcept {
  auto node = findOrCreateByKey(rootNode_, key);
  node = value;
  return OkStatus();
}

Status setNodeValue(YAML::Node* rootNode_, const std::string& key,
                    f32 value) noexcept {
  auto node = findOrCreateByKey(rootNode_, key);
  node = value;
  return OkStatus();
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
    if (i != 1) {
      v += Separator;
    }
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setInt3(std::string key, i32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 3; i++) {
    v += absl::StrFormat("%i", value[i]);
    if (i != 2) {
      v += Separator;
    }
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setInt4(std::string key, i32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 4; i++) {
    v += absl::StrFormat("%i", value[i]);
    if (i != 3) {
      v += Separator;
    }
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
    if (i != 1) {
      v += Separator;
    }
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setFloat3(std::string key, f32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 3; i++) {
    v += absl::StrFormat("%f", value[i]);
    if (i != 2) {
      v += Separator;
    }
  }
  return setNodeValue(&rootNode_, key, v);
}

Status AppConfigYamlImpl::setFloat4(std::string key, f32* value) noexcept {
  std::string v;
  for (auto i = 0; i < 4; i++) {
    v += absl::StrFormat("%f", value[i]);
    if (i != 3) {
      v += Separator;
    }
  }
  return setNodeValue(&rootNode_, key, v);
}

void AppConfigYamlImpl::flush() const noexcept {
  const std::string fp = filePath();
  std::filesystem::path p(fp);
  if (auto parent = p.parent_path(); !parent.empty()) {
    std::error_code ec;
    std::filesystem::create_directories(parent, ec);
    if (ec) {
      LOG(ERROR) << absl::StrFormat(
          "Failed to create AppConfig directories '%s': %s", parent.string(),
          ec.message());
      return;
    }
  }
  std::ofstream fout(fp);
  if (!fout.is_open()) {
    LOG(ERROR) << absl::StrFormat(
        "Failed to open AppConfig file '%s' for writing.", fp);
    return;
  }
  fout << rootNode_;
}

}  // namespace uinta
