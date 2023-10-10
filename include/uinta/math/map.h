#ifndef UINTA_MATH_MAP_H
#define UINTA_MATH_MAP_H

#include <uinta/types.h>

namespace uinta {

inline f32 map(f32 v, f32 src_min, f32 src_max, f32 dst_min, f32 dst_max) {
  if (src_max == src_min)  // Avoid divide by zero:
    return dst_min;
  auto perc = (v - src_min) / (src_max - src_min);
  return perc * (dst_max - dst_min) + dst_min;
}

}  // namespace uinta

#endif  // UINTA_MATH_MAP_H
