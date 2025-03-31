#ifndef SRC_ENGINE_INCLUDE_UINTA_APP_CONFIG_YAML_H_
#define SRC_ENGINE_INCLUDE_UINTA_APP_CONFIG_YAML_H_

#include <yaml-cpp/node/node.h>

#include <optional>
#include <string>

#include "uinta/app_config.h"
#include "uinta/file.h"
#include "uinta/types.h"

namespace uinta {

class ArgsProcessor;

class AppConfigYamlImpl final : public AppConfig {
 public:
  explicit AppConfigYamlImpl(const ArgsProcessor*,
                             FileSystem* = FileSystemImpl::Instance()) noexcept;

  std::optional<std::string> getString(std::string) const noexcept override;
  std::optional<bool> getBoolean(std::string) const noexcept override;
  std::optional<i32> getInt(std::string) const noexcept override;
  std::optional<i32*> getInt2(std::string, i32*) const noexcept override;
  std::optional<i32*> getInt3(std::string, i32*) const noexcept override;
  std::optional<i32*> getInt4(std::string, i32*) const noexcept override;
  std::optional<f32> getFloat(std::string) const noexcept override;
  std::optional<f32*> getFloat2(std::string, f32*) const noexcept override;
  std::optional<f32*> getFloat3(std::string, f32*) const noexcept override;
  std::optional<f32*> getFloat4(std::string, f32*) const noexcept override;

  Status setString(std::string key, std::string value) noexcept override;
  Status setBoolean(std::string key, bool) noexcept override;
  Status setInt(std::string key, i32) noexcept override;
  Status setInt2(std::string key, i32*) noexcept override;
  Status setInt3(std::string key, i32*) noexcept override;
  Status setInt4(std::string key, i32*) noexcept override;
  Status setFloat(std::string key, f32) noexcept override;
  Status setFloat2(std::string key, f32*) noexcept override;
  Status setFloat3(std::string key, f32*) noexcept override;
  Status setFloat4(std::string key, f32*) noexcept override;

  void flush() const noexcept override;

 private:
  YAML::Node rootNode_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_APP_CONFIG_YAML_H_
