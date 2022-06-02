#ifndef UINTA_FILE_H
#define UINTA_FILE_H

void readFileRaw(const char* const relative_path, char* const buffer);
void readFileBinary(const char* const relative_path, char* const buffer);
unsigned int getFileSize(const char* const relative_path);

#endif // UINTA_FILE_H
