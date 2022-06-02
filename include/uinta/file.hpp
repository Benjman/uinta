#ifndef UINTA_FILE_H
#define UINTA_FILE_H

void read_file_raw(const char* const relative_path, char* const buffer);
void read_file_binary(const char* const relative_path, char* const buffer);
unsigned int get_file_size(const char* const relative_path);

#endif // UINTA_FILE_H
