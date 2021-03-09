#ifndef UINTA_FILE_H
#define UINTA_FILE_H

#include "file_types.h"

namespace uinta {

	class FileDto;

	class File {
		file_size _contentLength;
		char *_contents;

		File(const char *contents, file_size contentLength);

	public:
		static File requestFile(const char *path);

		~File() {
			delete[] _contents;
		}

		[[nodiscard]] file_size getContentLength() const {
			return _contentLength;
		}

		[[nodiscard]] const char *getContents() const {
			return _contents;
		}

	}; // class File

} // namespace uinta

#endif //UINTA_FILE_H
