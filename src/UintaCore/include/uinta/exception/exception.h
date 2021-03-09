#ifndef UINTA_EXCEPTION_H
#define UINTA_EXCEPTION_H

#include <exception>

namespace uinta {

	struct Exception : public std::exception {
		explicit Exception(const char *message) : _message((char *) message) {}

		[[nodiscard]] const char *what() const noexcept override {
			return _message;
		}

		~Exception() override {
			delete[] _message;
		}

	protected:
		char *_message;

	}; // class Exception

} // namespace Uinta



#endif //UINTA_EXCEPTION_H
