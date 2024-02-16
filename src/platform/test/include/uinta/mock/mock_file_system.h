#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_FILE_SYSTEM_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_FILE_SYSTEM_H_

#include <functional>
#include <string>
#include <vector>

#include "uinta/file.h"

namespace uinta {

struct MockFileSystem : FileSystem {
  MockFileSystem() noexcept : FileSystem() {}

  std::function<bool(std::filesystem::path)> onExists = [](auto) {
    return true;
  };
  bool exists(std::filesystem::path path) const noexcept {
    return onExists(path);
  }

  std::function<bool(std::filesystem::path)> onIsDirectory = [](auto) {
    return true;
  };
  bool isDirectory(std::filesystem::path path) const noexcept {
    return onIsDirectory(path);
  }

  std::function<void()> onClose = []() {};
  void close() noexcept { return onClose(); }

  std::function<Status(std::filesystem::path src, std::filesystem::path dst,
                       std::filesystem::copy_options)>
      onCopy = [](const auto&, auto, auto) { return OkStatus(); };
  Status copy(std::filesystem::path src, std::filesystem::path dst,
              std::filesystem::copy_options options) noexcept {
    return onCopy(src, dst, options);
  }

  std::function<Status(std::filesystem::path, std::ios::openmode)> onOpen =
      [](const auto&, auto) { return OkStatus(); };
  Status open(std::filesystem::path path, std::ios::openmode mode) noexcept {
    return onOpen(path, mode);
  }

  std::function<bool()> onIsOpen = []() { return false; };
  bool isOpen() const noexcept { return onIsOpen(); }

  std::function<std::optional<std::string>()> onReadLine = []() { return ""; };
  std::optional<std::string> readLine() noexcept { return onReadLine(); }

  std::function<std::vector<std::string>()> onReadAllLines = []() {
    return std::vector<std::string>();
  };
  std::vector<std::string> readAllLines() noexcept { return onReadAllLines(); }

  std::function<std::string()> onReadAll = []() { return ""; };
  std::string readAll() noexcept { return onReadAll(); }

  std::function<std::vector<u8>()> onReadBinary = []() {
    return std::vector<u8>();
  };
  std::vector<u8> readBinary() noexcept { return onReadBinary(); }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_FILE_SYSTEM_H_
