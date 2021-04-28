#include <uinta/io/file.h>
#include <uinta/io/file_exception.h>

#include <fstream>

using namespace uinta;

File File::requestFile(const char *relativePath) {
	std::string path = UINTA_RES_PATH;
	path += relativePath;
	std::ifstream stream(path, std::ios::binary | std::ios::ate);
	if (stream.fail()) {
		throw FileNotFoundException(path);
	}
	std::streamsize contentLength = stream.tellg();
	stream.seekg(0, std::ios::beg);
	char contents[contentLength];
	stream.read(contents, contentLength);
	return File(contents, contentLength);
}

File::File(const char *contents, uint32_t contentLength): _contentLength(contentLength) {
	_contents = new char[contentLength + 1];
	strncpy(_contents, contents, contentLength - 1);
}

std::string File::getFilePath(const char *relativePath) {
	return std::string(UINTA_RES_PATH) + relativePath;
}
