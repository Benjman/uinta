#include "uinta/color.h"

#include "absl/random/random.h"
#include "absl/strings/str_format.h"
#include "uinta/types.h"

namespace uinta {
namespace color {

Color RandomColor() noexcept {
  static constexpr u8 ColorCount = 255;
  static constexpr std::array<Color, ColorCount + 1> colors = {
      Red50,          Red100,         Red200,         Red300,
      Red400,         Red500,         Red600,         Red700,
      Red800,         Red900,         RedA100,        RedA200,
      RedA400,        RedA700,        Pink50,         Pink100,
      Pink200,        Pink300,        Pink400,        Pink500,
      Pink600,        Pink700,        Pink800,        Pink900,
      PinkA100,       PinkA200,       PinkA400,       PinkA700,
      Purple50,       Purple100,      Purple200,      Purple300,
      Purple400,      Purple500,      Purple600,      Purple700,
      Purple800,      Purple900,      PurpleA100,     PurpleA200,
      PurpleA400,     PurpleA700,     DeepPurple50,   DeepPurple100,
      DeepPurple200,  DeepPurple300,  DeepPurple400,  DeepPurple500,
      DeepPurple600,  DeepPurple700,  DeepPurple800,  DeepPurple900,
      DeepPurpleA100, DeepPurpleA200, DeepPurpleA400, DeepPurpleA700,
      Indigo50,       Indigo100,      Indigo200,      Indigo300,
      Indigo400,      Indigo500,      Indigo600,      Indigo700,
      Indigo800,      Indigo900,      IndigoA100,     IndigoA200,
      IndigoA400,     IndigoA700,     Blue50,         Blue100,
      Blue200,        Blue300,        Blue400,        Blue500,
      Blue600,        Blue700,        Blue800,        Blue900,
      BlueA100,       BlueA200,       BlueA400,       BlueA700,
      LightBlue50,    LightBlue100,   LightBlue200,   LightBlue300,
      LightBlue400,   LightBlue500,   LightBlue600,   LightBlue700,
      LightBlue800,   LightBlue900,   LightBlueA100,  LightBlueA200,
      LightBlueA400,  LightBlueA700,  Cyan50,         Cyan100,
      Cyan200,        Cyan300,        Cyan400,        Cyan500,
      Cyan600,        Cyan700,        Cyan800,        Cyan900,
      CyanA100,       CyanA200,       CyanA400,       CyanA700,
      Teal50,         Teal100,        Teal200,        Teal300,
      Teal400,        Teal500,        Teal600,        Teal700,
      Teal800,        Teal900,        TealA100,       TealA200,
      TealA400,       TealA700,       Green50,        Green100,
      Green200,       Green300,       Green400,       Green500,
      Green600,       Green700,       Green800,       Green900,
      GreenA100,      GreenA200,      GreenA400,      GreenA700,
      LightGreen50,   LightGreen100,  LightGreen200,  LightGreen300,
      LightGreen400,  LightGreen500,  LightGreen600,  LightGreen700,
      LightGreen800,  LightGreen900,  LightGreenA100, LightGreenA200,
      LightGreenA400, LightGreenA700, Lime50,         Lime100,
      Lime200,        Lime300,        Lime400,        Lime500,
      Lime600,        Lime700,        Lime800,        Lime900,
      LimeA100,       LimeA200,       LimeA400,       LimeA700,
      Yellow50,       Yellow100,      Yellow200,      Yellow300,
      Yellow400,      Yellow500,      Yellow600,      Yellow700,
      Yellow800,      Yellow900,      YellowA100,     YellowA200,
      YellowA400,     YellowA700,     Amber50,        Amber100,
      Amber200,       Amber300,       Amber400,       Amber500,
      Amber600,       Amber700,       Amber800,       Amber900,
      AmberA100,      AmberA200,      AmberA400,      AmberA700,
      Orange50,       Orange100,      Orange200,      Orange300,
      Orange400,      Orange500,      Orange600,      Orange700,
      Orange800,      Orange900,      OrangeA100,     OrangeA200,
      OrangeA400,     OrangeA700,     DeepOrange50,   DeepOrange100,
      DeepOrange200,  DeepOrange300,  DeepOrange400,  DeepOrange500,
      DeepOrange600,  DeepOrange700,  DeepOrange800,  DeepOrange900,
      DeepOrangeA100, DeepOrangeA200, DeepOrangeA400, DeepOrangeA700,
      Brown50,        Brown100,       Brown200,       Brown300,
      Brown400,       Brown500,       Brown600,       Brown700,
      Brown800,       Brown900,       Gray50,         Gray100,
      Gray200,        Gray300,        Gray400,        Gray500,
      Gray600,        Gray700,        Gray800,        Gray900,
      BlueGray50,     BlueGray100,    BlueGray200,    BlueGray300,
      BlueGray400,    BlueGray500,    BlueGray600,    BlueGray700,
      BlueGray800,    BlueGray900,    Black,          White,
  };
  absl::BitGen bitgen;
  return colors.at(bitgen() % (ColorCount + 1));
}

std::string to_string(Color color) noexcept {
  return absl::StrFormat("Color(%f, %f, %f)", color.r, color.g, color.b);
}

}  // namespace color
}  // namespace uinta
