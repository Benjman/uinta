#ifndef UINTA_TEST_FILE_MANAGER_HPP
#define UINTA_TEST_FILE_MANAGER_HPP

#include <functional>
#include <uinta/file_manager.hpp>

namespace uinta {

class MockFileManager : public FileManager {
 protected:
  std::function<void()> on_loadFileBinary;
  void loadFileBinary(const file_t* const handle) override {
    if (on_loadFileBinary) on_loadFileBinary();
  }

  std::function<void()> on_loadFileText;
  void loadFileText(const file_t* const handle) override {
    if (on_loadFileText) on_loadFileText();
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_FILE_MANAGER_HPP
