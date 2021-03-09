#ifndef UINTA_OS_H
#define UINTA_OS_H

#include "types.h"

/***
 * Hardware specific data, like available memory, max threads, etc.
 */

#include <thread>

inline size_t uintaGetHardwareConcurrencyCount() {
	return std::thread::hardware_concurrency();
}

inline const char *uintaGetOsName() {

#ifdef _WIN32
	return "Windows 32-bit";
#elif _WIN64
	return "Windows 64-bit";
#elif __APPLE__ || __MACH__
	return "Mac OSX";
#elif __linux__
	return "Linux";
#elif __FreeBSD__
	return "FreeBSD";
#elif __unix || __unix__
	return "Unix";
#else
	return "Other";
#endif

}

#endif //UINTA_OS_H
