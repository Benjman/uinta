#include <file.hpp>
#include <cfg.hpp>

#include <ios>
#include <istream>
#include <cstring>

#include <stb_truetype.h>

extern void read_file_internal(const char *filepath, std::ios::openmode mode, char *buffer) {
    std::ifstream file(filepath, std::ios::in | mode | std::ios::ate);
    if (!file) {
        printf("something went wrong trying to open file %s\n", filepath);
        return;
    }
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read(buffer, length);
    file.close();
    if (buffer[length] != '\0') {
        printf("null terminating string for file buffer from path %s\n", filepath);
        buffer[length] = '\0';
    }
}

extern void read_file_raw(const char *relative_path, char *buffer) {
    const unsigned int path_len = strlen(RES_PATH) + strlen(relative_path);
    char abs_path[path_len];
    memset(abs_path, 0, path_len);
    strcat(abs_path, RES_PATH);
    strcat(abs_path, relative_path);
    read_file_internal(abs_path, std::ios::in, buffer);
}

extern void read_file_binary(const char *relative_path, char *buffer) {
    const unsigned int path_len = strlen(RES_PATH) + strlen(relative_path);
    char abs_path[path_len];
    memset(abs_path, 0, path_len);
    strcat(abs_path, RES_PATH);
    strcat(abs_path, relative_path);
    read_file_internal(abs_path, std::ios::binary, buffer);
}
