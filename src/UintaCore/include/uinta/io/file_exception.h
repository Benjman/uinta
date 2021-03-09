#ifndef UINTA_FILE_EXCEPTION_H
#define UINTA_FILE_EXCEPTION_H

#include <uinta/exception/exception.h>

#include <string>
#include <cstring>

namespace uinta {

	struct FileNotFoundException : public Exception {
		explicit FileNotFoundException(const std::string &path) : FileNotFoundException(path.c_str()) {}

		explicit FileNotFoundException(const char *path) : Exception(path) {
			_message = new char[strlen(prefix) + strlen(path) + 2];
			strcpy(_message, prefix);
			strcat(_message, path);
		}

	private:
		static const char *prefix;

	}; // struct FileNotFoundException

} // namespace uinta

const char *uinta::FileNotFoundException::prefix = "File not found: ";

#endif //UINTA_FILE_EXCEPTION_H
