#ifndef UINTA_TYPES_H
#define UINTA_TYPES_H

#include <cmath>
#include <cstdint>

namespace uinta {

// Unsigned integer types
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// Signed integer types
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

// Floating point types
using f32 = std::float_t;
using f64 = std::double_t;

// Char types
using uchar = unsigned char;

}  // namespace uinta

#endif  // UINTA_TYPES_H
