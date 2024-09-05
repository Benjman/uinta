#include "uinta/reflect.h"

#ifdef __GNUG__
#include <cxxabi.h>

#include <cstdlib>
#include <memory>

namespace uinta {

std::string demangleTypeName(std::string name) noexcept {
  int status = 0;

  std::unique_ptr<char, void (*)(void*)> res{
      abi::__cxa_demangle(name.c_str(), NULL, NULL, &status), std::free};

  return (status == 0) ? res.get() : name;
}

}  // namespace uinta

#else

namespace uinta {

std::string demangleTypeName(std::string name) noexcept { return name; }

}  // namespace uinta

#endif  // __GNUG__
