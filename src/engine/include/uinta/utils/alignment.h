#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_

#include "uinta/types.h"

namespace uinta {

namespace _ {

struct Shifts final {
  static constexpr u8 Horizontal = 0;
  static constexpr u8 Vertical = 3;
};

struct Masks final {
  static constexpr u8 Horizontal = 0x7 << Shifts::Horizontal;
  static constexpr u8 Vertical = 0x7 << Shifts::Vertical;

  static constexpr u8 Left = 1 << 0;
  static constexpr u8 Center = 1 << 1;
  static constexpr u8 Right = 1 << 2;

  static constexpr u8 Top = 1 << 0;
  static constexpr u8 Middle = 1 << 1;
  static constexpr u8 Bottom = 1 << 2;
};

}  // namespace _

enum class Alignment {
  BottomCenter = ((_::Masks::Bottom << _::Shifts::Vertical) |
                  (_::Masks::Center << _::Shifts::Horizontal)),
  BottomLeft = ((_::Masks::Bottom << _::Shifts::Vertical) |
                (_::Masks::Left << _::Shifts::Horizontal)),
  BottomRight = ((_::Masks::Bottom << _::Shifts::Vertical) |
                 (_::Masks::Right << _::Shifts::Horizontal)),
  MiddleCenter = ((_::Masks::Middle << _::Shifts::Vertical) |
                  (_::Masks::Center << _::Shifts::Horizontal)),
  MiddleLeft = ((_::Masks::Middle << _::Shifts::Vertical) |
                (_::Masks::Left << _::Shifts::Horizontal)),
  MiddleRight = ((_::Masks::Middle << _::Shifts::Vertical) |
                 (_::Masks::Right << _::Shifts::Horizontal)),
  TopCenter = ((_::Masks::Top << _::Shifts::Vertical) |
               (_::Masks::Center << _::Shifts::Horizontal)),
  TopLeft = ((_::Masks::Top << _::Shifts::Vertical) |
             (_::Masks::Left << _::Shifts::Horizontal)),
  TopRight = ((_::Masks::Top << _::Shifts::Vertical) |
              (_::Masks::Right << _::Shifts::Horizontal)),
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_
