#ifndef UINTA_TEST_FILE_MANAGER_HPP
#define UINTA_TEST_FILE_MANAGER_HPP

#include <uinta/file_manager.hpp>

namespace uinta {

class TestFileManager : public FileManager {
 protected:
  bool loadFileBinaryCalled = false;
  void loadFileBinary(const file_t* const handle) override {
    loadFileBinaryCalled = true;
  }

  bool loadFileTextCalled = false;
  void loadFileText(const file_t* const handle) override {
    loadFileTextCalled = true;
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_FILE_MANAGER_HPP
