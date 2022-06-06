#include <uinta/cfg.hpp>
#include <uinta/logging.hpp>
#include <uinta/file.hpp>

#include <ios>
#include <istream>
#include <cstring>
#include <fstream>

namespace uinta::internal {

bool isFileValid(const std::ifstream &file, const std::string &path) {
  if (file && file.good()) {
    return true;
  }
  SPDLOG_ERROR("isFileValid() - failed to open file {}", path);
  return false;
}

void readFileInternal(const std::string &path, std::ios::openmode mode, char *const buffer) {
  std::ifstream file(path, std::ios::in | mode | std::ios::ate);
  if (!isFileValid(file, path))
    return;
  int length = file.tellg();
  file.seekg(0, std::ios::beg);
  file.read(buffer, length);
  file.close();
}

} // namespace uinta::internal

void uinta::readFileRaw(const char *const relativePath, char *const buffer) {
  internal::readFileInternal(std::string(RES_PATH) + relativePath, std::ios::in, buffer);
}

void uinta::readFileBinary(const char *const relativePath, char *const buffer) {
  internal::readFileInternal(std::string(RES_PATH) + relativePath, std::ios::binary, buffer);
}

unsigned int uinta::getFileSize(const char *const relativePath) {
  std::string path = std::string(RES_PATH) + relativePath;
  std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
  if (!internal::isFileValid(file, path))
    return 0;
  return file.tellg();
}
