#include <file.hpp>
#include <uinta/cfg.hpp>

#include <ios>
#include <istream>
#include <cstring>
#include <fstream>

#include <logging.hpp>

namespace internal {

void read_file_internal(const std::string& path, std::ios::openmode mode, char* const buffer) {
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

void read_file_raw(const char* const relative_path, char* const buffer) {
    std::string abs_path = std::string(RES_PATH) + relative_path;
    internal::read_file_internal(abs_path, std::ios::in, buffer);
}

void read_file_binary(const char* const relative_path, char* const buffer) {
    std::string abs_path = std::string(RES_PATH) + relative_path;
    internal::read_file_internal(abs_path, std::ios::binary, buffer);
}

unsigned int get_file_size(const char* const relative_path) {
    std::string abs_path = std::string(RES_PATH) + relative_path;
    std::ifstream in(abs_path, std::ifstream::ate | std::ifstream::binary);
    if (!in || !in.good()) {
        SPDLOG_ERROR("read_file_internal - failed to open file {}", abs_path);
        return 0;
    }
    return in.tellg();
}
