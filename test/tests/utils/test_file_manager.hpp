#ifndef UINTA_TEST_FILE_MANAGER_HPP
#define UINTA_TEST_FILE_MANAGER_HPP

#include <functional>
#include <uinta/file_manager.hpp>

namespace uinta {

class TestFileManager : public FileManager {
 public:
  void onLoadFileBinary(const std::function<void()> func) {
    m_on_loadFileBinary = func;
  }

  void onLoadFileText(const std::function<void()> func) {
    m_on_loadFileText = func;
  }

 protected:
  std::function<void()> m_on_loadFileBinary;
  bool loadFileBinaryCalled = false;
  void loadFileBinary(const file_t* const handle) override {
    loadFileBinaryCalled = true;
    m_on_loadFileBinary();
  }

  bool loadFileTextCalled = false;
  std::function<void()> m_on_loadFileText;
  void loadFileText(const file_t* const handle) override {
    loadFileTextCalled = true;
    m_on_loadFileText();
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_FILE_MANAGER_HPP
