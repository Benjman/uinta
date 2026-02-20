#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_ALIGNMENT_H_

#include "uinta/types.h"

namespace uinta {

namespace _ {

struct Shifts final {
  using value_type = u8;

  static constexpr value_type Horizontal = 0;
  static constexpr value_type Vertical = 3;
};

struct Masks final {
  using value_type = u8;

  static constexpr value_type Horizontal = 0x7 << Shifts::Horizontal;
  static constexpr value_type Vertical = 0x7 << Shifts::Vertical;

  static constexpr value_type Left = 1 << 0;
  static constexpr value_type Center = 1 << 1;
  static constexpr value_type Right = 1 << 2;

  static constexpr value_type Top = 1 << 0;
  static constexpr value_type Middle = 1 << 1;
  static constexpr value_type Bottom = 1 << 2;
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
