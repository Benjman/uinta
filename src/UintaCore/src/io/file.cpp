#include <uinta/io/file.h>
#include <uinta/io/file_exception.h>

#include <fstream>

uinta::File uinta::File::requestFile(const char *path) {
	std::ifstream stream(path, std::ios::binary | std::ios::ate);
	if (stream.fail()) {
		throw FileNotFoundException(path);
	}
	size_t contentLength = stream.tellg();
	stream.seekg(0, std::ios::beg);
	char contents[contentLength];
	stream.read(contents, contentLength);
	return File(contents, contentLength);
}

uinta::File::File(const char *contents, file_size_t contentLength): _contentLength(contentLength) {
	_contents = new char[contentLength + 1];
	strcpy(_contents, contents);
}
