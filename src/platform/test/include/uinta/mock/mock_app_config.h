#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_APP_CONFIG_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_APP_CONFIG_H_

#include <functional>
#include <optional>
#include <string>

#include "uinta/app_config.h"
#include "uinta/args.h"
#include "uinta/mock/mock_file_system.h"

namespace uinta {

struct MockAppConfig : AppConfig {
  MockFileSystem fs_;
  ArgsProcessor args_ = ArgsProcessor(0, nullptr);

  explicit MockAppConfig(const ArgsProcessor* args = nullptr,
                         FileSystem* fs = nullptr) noexcept
      : AppConfig("mock.cfg", args ? args : &args_, fs ? fs : &fs_) {}

  std::function<std::optional<std::string>(std::string)> onGetString =
      [](auto) { return std::nullopt; };
  std::optional<std::string> getString(
      std::string key) const noexcept override {
    return onGetString(key);
  }

  std::function<std::optional<bool>(std::string)> onGetBoolean = [](auto) {
    return std::nullopt;
  };
  std::optional<bool> getBoolean(std::string key) const noexcept override {
    return onGetBoolean(key);
  }

  std::function<std::optional<i32>(std::string)> onGetInt = [](auto) {
    return std::nullopt;
  };
  std::optional<i32> getInt(std::string key) const noexcept override {
    return onGetInt(key);
  }

  std::function<std::optional<i32*>(std::string, i32*)> onGetInt2 =
      [](auto, auto*) { return std::nullopt; };
  std::optional<i32*> getInt2(std::string key,
                              i32* ptr) const noexcept override {
    return onGetInt2(key, ptr);
  }

  std::function<std::optional<i32*>(std::string, i32*)> onGetInt3 =
      [](auto, auto*) { return std::nullopt; };
  std::optional<i32*> getInt3(std::string key,
                              i32* ptr) const noexcept override {
    return onGetInt3(key, ptr);
  }

  std::function<std::optional<i32*>(std::string, i32*)> onGetInt4 =
      [](auto, auto*) { return std::nullopt; };
  std::optional<i32*> getInt4(std::string key,
                              i32* ptr) const noexcept override {
    return onGetInt4(key, ptr);
  }

  std::function<std::optional<f32>(std::string)> onGetFloat = [](auto) {
    return std::nullopt;
  };
  std::optional<f32> getFloat(std::string key) const noexcept override {
    return onGetFloat(key);
  }

  std::function<std::optional<f32*>(std::string, f32*)> onGetFloat2 =
      [](auto, auto*) { return std::nullopt; };
  std::optional<f32*> getFloat2(std::string key,
                                f32* ptr) const noexcept override {
    return onGetFloat2(key, ptr);
  }

  std::function<std::optional<f32*>(std::string, f32*)> onGetFloat3 =
      [](auto, auto*) { return std::nullopt; };
  std::optional<f32*> getFloat3(std::string key,
                                f32* ptr) const noexcept override {
    return onGetFloat3(key, ptr);
  }

  std::function<std::optional<f32*>(std::string, f32*)> onGetFloat4 =
      [](auto, auto*) { return std::nullopt; };
  std::optional<f32*> getFloat4(std::string key,
                                f32* ptr) const noexcept override {
    return onGetFloat4(key, ptr);
  }

  std::function<Status(std::string, std::string)> onSetString =
      [](auto, std::string) { return OkStatus(); };
  Status setString(std::string key, std::string value) noexcept override {
    return onSetString(key, value);
  }

  std::function<Status(std::string, bool)> onSetBoolean = [](auto, bool) {
    return OkStatus();
  };
  Status setBoolean(std::string key, bool value) noexcept override {
    return onSetBoolean(key, value);
  }

  std::function<Status(std::string, i32)> onSetInt = [](auto, i32) {
    return OkStatus();
  };
  Status setInt(std::string key, i32 value) noexcept override {
    return onSetInt(key, value);
  }

  std::function<Status(std::string, i32*)> onSetInt2 = [](auto, i32*) {
    return OkStatus();
  };
  Status setInt2(std::string key, i32* value) noexcept override {
    return onSetInt2(key, value);
  }

  std::function<Status(std::string, i32*)> onSetInt3 = [](auto, i32*) {
    return OkStatus();
  };
  Status setInt3(std::string key, i32* value) noexcept override {
    return onSetInt3(key, value);
  }

  std::function<Status(std::string, i32*)> onSetInt4 = [](auto, i32*) {
    return OkStatus();
  };
  Status setInt4(std::string key, i32* value) noexcept override {
    return onSetInt4(key, value);
  }

  std::function<Status(std::string, f32)> onSetFloat = [](auto, f32) {
    return OkStatus();
  };
  Status setFloat(std::string key, f32 value) noexcept override {
    return onSetFloat(key, value);
  }

  std::function<Status(std::string, f32*)> onSetFloat2 = [](auto, f32*) {
    return OkStatus();
  };
  Status setFloat2(std::string key, f32* value) noexcept override {
    return onSetFloat2(key, value);
  }

  std::function<Status(std::string, f32*)> onSetFloat3 = [](auto, f32*) {
    return OkStatus();
  };
  Status setFloat3(std::string key, f32* value) noexcept override {
    return onSetFloat3(key, value);
  }

  std::function<Status(std::string, f32*)> onSetFloat4 = [](auto, f32*) {
    return OkStatus();
  };
  Status setFloat4(std::string key, f32* value) noexcept override {
    return onSetFloat4(key, value);
  }

  std::function<void()> onFlush = []() {};
  void flush() const noexcept override { onFlush(); }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_APP_CONFIG_H_
