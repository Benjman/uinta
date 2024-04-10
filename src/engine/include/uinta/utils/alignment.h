#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_

#include "uinta/types.h"

namespace uinta {

namespace _ {
static constexpr u8 HorizontalShift = 0;
static constexpr u8 HorizontalMask = 0x7 << HorizontalShift;
static constexpr u8 VerticalShift = 3;
static constexpr u8 VerticalMask = 0x7 << VerticalShift;

static constexpr u8 LeftMask = 1 << 0;
static constexpr u8 CenterMask = 1 << 1;
static constexpr u8 RightMask = 1 << 2;

static constexpr u8 TopMask = 1 << 0;
static constexpr u8 MiddleMask = 1 << 1;
static constexpr u8 BottomMask = 1 << 2;
}  // namespace _

enum class Alignment {
  BottomCenter = ((_::BottomMask << _::VerticalShift) |
                  (_::CenterMask << _::HorizontalShift)),
  BottomLeft = ((_::BottomMask << _::VerticalShift) |
                (_::LeftMask << _::HorizontalShift)),
  BottomRight = ((_::BottomMask << _::VerticalShift) |
                 (_::RightMask << _::HorizontalShift)),
  MiddleCenter = ((_::MiddleMask << _::VerticalShift) |
                  (_::CenterMask << _::HorizontalShift)),
  MiddleLeft = ((_::MiddleMask << _::VerticalShift) |
                (_::LeftMask << _::HorizontalShift)),
  MiddleRight = ((_::MiddleMask << _::VerticalShift) |
                 (_::RightMask << _::HorizontalShift)),
  TopCenter = ((_::TopMask << _::VerticalShift) |
               (_::CenterMask << _::HorizontalShift)),
  TopLeft =
      ((_::TopMask << _::VerticalShift) | (_::LeftMask << _::HorizontalShift)),
  TopRight =
      ((_::TopMask << _::VerticalShift) | (_::RightMask << _::HorizontalShift)),
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_
