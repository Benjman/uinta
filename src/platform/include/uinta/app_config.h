#ifndef SRC_PLATFORM_INCLUDE_UINTA_APP_CONFIG_H_
#define SRC_PLATFORM_INCLUDE_UINTA_APP_CONFIG_H_

#include <optional>
#include <string>

#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

class FileSystem;
class ArgsProcessor;

class AppConfig {
 public:
  virtual std::optional<std::string> getString(std::string) const noexcept = 0;
  virtual std::optional<bool> getBoolean(std::string) const noexcept = 0;
  virtual std::optional<i32> getInt(std::string) const noexcept = 0;
  virtual std::optional<i32*> getInt2(std::string, i32*) const noexcept = 0;
  virtual std::optional<i32*> getInt3(std::string, i32*) const noexcept = 0;
  virtual std::optional<i32*> getInt4(std::string, i32*) const noexcept = 0;
  virtual std::optional<f32> getFloat(std::string) const noexcept = 0;
  virtual std::optional<f32*> getFloat2(std::string, f32*) const noexcept = 0;
  virtual std::optional<f32*> getFloat3(std::string, f32*) const noexcept = 0;
  virtual std::optional<f32*> getFloat4(std::string, f32*) const noexcept = 0;

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

  virtual void flush() const noexcept = 0;

  Status status() const noexcept { return status_; }

  std::string filePath() const noexcept { return filePath_; }

 protected:
  explicit AppConfig(std::string filename, const ArgsProcessor*,
                     FileSystem*) noexcept;

  void status(Status status) noexcept {
    if (status_.ok()) {
      status_ = status;
    }
  }

 private:
  Status status_ = OkStatus();
  FileSystem* fs_ = nullptr;
  std::string filePath_ = "";
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_APP_CONFIG_H_
