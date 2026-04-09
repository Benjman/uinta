#ifndef SRC_PLATFORM_INCLUDE_UINTA_APP_CONFIG_H_
#define SRC_PLATFORM_INCLUDE_UINTA_APP_CONFIG_H_

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

class ArgsProcessor;

class AppConfig {
 public:
  [[nodiscard]]
  virtual std::optional<std::string> getString(std::string) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<bool> getBoolean(std::string) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<i32> getInt(std::string) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<i32*> getInt2(std::string, i32*) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<i32*> getInt3(std::string, i32*) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<i32*> getInt4(std::string, i32*) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<f32> getFloat(std::string) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<f32*> getFloat2(std::string, f32*) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<f32*> getFloat3(std::string, f32*) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<f32*> getFloat4(std::string, f32*) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<std::vector<std::string>> getStringList(std::string) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<std::vector<i32>> getIntList(std::string) const noexcept = 0;
  [[nodiscard]]
  virtual std::optional<std::vector<f32>> getFloatList(std::string) const noexcept = 0;

  virtual Status setString(std::string, std::string) noexcept = 0;
  virtual Status setBoolean(std::string, bool) noexcept = 0;
  virtual Status setInt(std::string, i32) noexcept = 0;
  virtual Status setInt2(std::string, i32*) noexcept = 0;
  virtual Status setInt3(std::string, i32*) noexcept = 0;
  virtual Status setInt4(std::string, i32*) noexcept = 0;
  virtual Status setFloat(std::string, f32) noexcept = 0;
  virtual Status setFloat2(std::string, f32*) noexcept = 0;
  virtual Status setFloat3(std::string, f32*) noexcept = 0;
  virtual Status setFloat4(std::string, f32*) noexcept = 0;
  virtual Status setStringList(std::string, const std::vector<std::string>&) noexcept = 0;
  virtual Status setIntList(std::string, const std::vector<i32>&) noexcept = 0;
  virtual Status setFloatList(std::string, const std::vector<f32>&) noexcept = 0;

  virtual void flush() const noexcept = 0;

  // Schema version for config migration (0 = legacy/absent)
  static constexpr i32 CurrentSchemaVersion = 1;
  [[nodiscard]] virtual i32 schemaVersion() const noexcept = 0;

  [[nodiscard]] Status status() const noexcept { return status_; }

  [[nodiscard]] std::string filePath() const noexcept { return filePath_; }

 protected:
  explicit AppConfig(const std::filesystem::path& filePath, const ArgsProcessor*) noexcept;

  void status(const Status& status) noexcept {
    if (status_.ok()) {
      status_ = status;
    }
  }

 private:
  Status status_ = OkStatus();
  std::string filePath_;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_APP_CONFIG_H_
