#include <file.hpp>
#include <cfg.hpp>

#include <ios>
#include <istream>
#include <cstring>
#include <fstream>

void read_file_internal(const char* const filepath, std::ios::openmode mode, char* const buffer) {
    std::ifstream file(filepath, std::ios::in | mode | std::ios::ate);
    if (!file || !file.good()) {
        printf("something went wrong trying to open file %s\n", filepath);
        return;
    }
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read(buffer, length);
    file.close();
}

void read_file_raw(const char* const relative_path, char* const buffer) {
    const unsigned int path_len = strlen(RES_PATH) + strlen(relative_path);
    char abs_path[path_len];
    memset(abs_path, 0, path_len);
    strcat(abs_path, RES_PATH);
    strcat(abs_path, relative_path);
    read_file_internal(abs_path, std::ios::in, buffer);
}

void read_file_binary(const char* const relative_path, char* const buffer) {
    const unsigned int path_len = strlen(RES_PATH) + strlen(relative_path);
    char abs_path[path_len];
    memset(abs_path, 0, path_len);
    strcat(abs_path, RES_PATH);
    strcat(abs_path, relative_path);
    read_file_internal(abs_path, std::ios::binary, buffer);
}

unsigned int get_file_size(const char* const relative_path) {
    const unsigned int path_len = strlen(RES_PATH) + strlen(relative_path);
    char abs_path[path_len];
    memset(abs_path, 0, path_len);
    strcat(abs_path, RES_PATH);
    strcat(abs_path, relative_path);
    std::ifstream in(abs_path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}
