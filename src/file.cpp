#include <uinta/cfg.hpp>
#include <uinta/logging.hpp>
#include <uinta/file.hpp>

#include <ios>
#include <istream>
#include <cstring>
#include <fstream>

namespace internal {

void readFileInternal(const std::string& path, std::ios::openmode mode, char* const buffer) {
    std::ifstream file(path, std::ios::in | mode | std::ios::ate);
    if (!file || !file.good()) {
        SPDLOG_ERROR("read_file_internal - failed to open file {}", path);
        return;
    }
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read(buffer, length);
    file.close();
}

}

void readFileRaw(const char* const relative_path, char* const buffer) {
    std::string abs_path = std::string(RES_PATH) + relative_path;
    internal::readFileInternal(abs_path, std::ios::in, buffer);
}

void readFileBinary(const char* const relative_path, char* const buffer) {
    std::string abs_path = std::string(RES_PATH) + relative_path;
    internal::readFileInternal(abs_path, std::ios::binary, buffer);
}

unsigned int getFileSize(const char* const relative_path) {
    std::string abs_path = std::string(RES_PATH) + relative_path;
    std::ifstream in(abs_path, std::ifstream::ate | std::ifstream::binary);
    if (!in || !in.good()) {
        SPDLOG_ERROR("read_file_internal - failed to open file {}", abs_path);
        return 0;
    }
    return in.tellg();
}
