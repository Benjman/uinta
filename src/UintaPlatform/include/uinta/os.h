#ifndef UINTA_OS_H
#define UINTA_OS_H

#include "types.h"

/***
 * Hardware specific data, like available memory, max threads, etc.
 */

#ifdef _WIN32
#define UINTA_OS "Windows 32-bit"
#elif _WIN64
#define UINTA_OS "Windows 64-bit"
#elif __APPLE__ || __MACH__
#define UINTA_OS "Mac OSX"
#elif __linux__
#define UINTA_OS "Linux"
#elif __FreeBSD__
#define UINTA_OS "FreeBSD"
#elif __unix || __unix__
#define UINTA_OS "Unix"
#else
#define UINTA_OS "Other"
#endif

#include <thread>

inline size_t uintaGetHardwareConcurrencyCount() {
	return std::thread::hardware_concurrency();
}

#endif //UINTA_OS_H
