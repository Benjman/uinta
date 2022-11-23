#ifndef UINTA_UTILS_FORMATTERS_HPP
#define UINTA_UTILS_FORMATTERS_HPP

#include <cmath>
#include <string>
#include <uinta/fwd.hpp>

namespace uinta {

#define DEFAULT_DECIMAL_PLACES 2

inline std::string formatMemory(int count, uint decimalPlaces = DEFAULT_DECIMAL_PLACES);
inline std::string removeTrailingZeroes(std::string str);

inline std::string formatMemory(int count, uint decimalPlaces) {
  if (count == 1) return "1 byte";

  float value_f = count;
  std::string label = " ";
  auto abs = std::abs(value_f);
  auto pow = std::pow(10, decimalPlaces);

  if (abs >= GIGABYTES(1)) {
    value_f /= (float)GIGABYTES(1);
    label += "GB";
  } else if (abs >= MEGABYTES(1)) {
    value_f /= (float)(MEGABYTES(1));
    label += "MB";
  } else if (abs >= KILOBYTES(1)) {
    value_f /= (float)KILOBYTES(1);
    label += "KB";
  } else {
    label += "bytes";
  }

  value_f *= pow;
  value_f = std::round(value_f);
  value_f /= pow;

  return removeTrailingZeroes(std::to_string(value_f)) + label;
}

inline std::string removeTrailingZeroes(std::string str) {
  if (str.find('.') != std::string::npos) {
    str = str.substr(0, str.find_last_not_of('0') + 1);
    if (str.find('.') == str.size() - 1) {
      str = str.substr(0, str.size() - 1);
    }
  }
  return str;
}

}  // namespace uinta

#endif  // UINTA_UTILS_FORMATTERS_HPP