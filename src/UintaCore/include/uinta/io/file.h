#ifndef UINTA_FILE_H
#define UINTA_FILE_H

#include <uinta/types.h>

namespace uinta {

	class File {
		uint32_t _contentLength;
		char *_contents;

		File(const char *contents, uint32_t contentLength);

	public:
		static File requestFile(const char *path);

		~File() {
			delete[] _contents;
		}

		[[nodiscard]] uint32_t getContentLength() const {
			return _contentLength;
		}

		[[nodiscard]] const char *getContents() const {
			return _contents;
		}

	}; // class File

} // namespace uinta

#endif //UINTA_FILE_H
