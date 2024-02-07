#ifndef SRC_PLATFORM_INCLUDE_UINTA_TYPES_H_
#define SRC_PLATFORM_INCLUDE_UINTA_TYPES_H_

#include <cstddef>
#include <cstdint>

namespace uinta {

// Signed integer types
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

// Unsigned integer types
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// Floating point types
using f32 = float;
using f64 = double;

using count_t = u64;
using size_t = std::size_t;
using time_t = f64;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_TYPES_H_
