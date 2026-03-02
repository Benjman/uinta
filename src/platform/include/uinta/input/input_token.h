#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_TOKEN_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_TOKEN_H_

#include "uinta/input/fwd.h"
#include "uinta/types.h"

namespace uinta {

// Bitfield Layout (32-bit token):
// Bits 0-15  (16 bits): Key codes or MouseButton codes (0-65535)
// Bits 16-19 (4 bits):  Action codes (0-15)
// Bits 20-27 (8 bits):  Modifier flags
// Bits 28-31 (4 bits):  Unassigned

// Key enum (bits 0-15) - strongly typed, uses GLFW-compatible values
enum class Key : u32 {
  Space = 32,         // 0x0020
  Apostrophe = 39,    // 0x0027
  Comma = 44,         // 0x002C
  Minus = 45,         // 0x002D
  Period = 46,        // 0x002E
  Slash = 47,         // 0x002F
  Num0 = 48,          // 0x0030
  Num1 = 49,          // 0x0031
  Num2 = 50,          // 0x0032
  Num3 = 51,          // 0x0033
  Num4 = 52,          // 0x0034
  Num5 = 53,          // 0x0035
  Num6 = 54,          // 0x0036
  Num7 = 55,          // 0x0037
  Num8 = 56,          // 0x0038
  Num9 = 57,          // 0x0039
  Semicolon = 59,     // 0x003B
  Equal = 61,         // 0x003D
  A = 65,             // 0x0041
  B = 66,             // 0x0042
  C = 67,             // 0x0043
  D = 68,             // 0x0044
  E = 69,             // 0x0045
  F = 70,             // 0x0046
  G = 71,             // 0x0047
  H = 72,             // 0x0048
  I = 73,             // 0x0049
  J = 74,             // 0x004A
  K = 75,             // 0x004B
  L = 76,             // 0x004C
  M = 77,             // 0x004D
  N = 78,             // 0x004E
  O = 79,             // 0x004F
  P = 80,             // 0x0050
  Q = 81,             // 0x0051
  R = 82,             // 0x0052
  S = 83,             // 0x0053
  T = 84,             // 0x0054
  U = 85,             // 0x0055
  V = 86,             // 0x0056
  W = 87,             // 0x0057
  X = 88,             // 0x0058
  Y = 89,             // 0x0059
  Z = 90,             // 0x005A
  LeftBracket = 91,   // 0x005B
  Backslash = 92,     // 0x005C
  RightBracket = 93,  // 0x005D
  GraveAccent = 96,   // 0x0060

  // Function and special keys
  Escape = 256,       // 0x0100
  Enter = 257,        // 0x0101
  Tab = 258,          // 0x0102
  Backspace = 259,    // 0x0103
  Insert = 260,       // 0x0104
  Delete = 261,       // 0x0105
  Right = 262,        // 0x0106
  Left = 263,         // 0x0107
  Down = 264,         // 0x0108
  Up = 265,           // 0x0109
  PageUp = 266,       // 0x010A
  PageDown = 267,     // 0x010B
  Home = 268,         // 0x010C
  End = 269,          // 0x010D
  CapsLock = 280,     // 0x0118
  ScrollLock = 281,   // 0x0119
  NumLock = 282,      // 0x011A
  PrintScreen = 283,  // 0x011B
  Pause = 284,        // 0x011C
  F1 = 290,           // 0x0122
  F2 = 291,           // 0x0123
  F3 = 292,           // 0x0124
  F4 = 293,           // 0x0125
  F5 = 294,           // 0x0126
  F6 = 295,           // 0x0127
  F7 = 296,           // 0x0128
  F8 = 297,           // 0x0129
  F9 = 298,           // 0x012A
  F10 = 299,          // 0x012B
  F11 = 300,          // 0x012C
  F12 = 301,          // 0x012D
  F13 = 302,          // 0x012E
  F14 = 303,          // 0x012F
  F15 = 304,          // 0x0130
  F16 = 305,          // 0x0131
  F17 = 306,          // 0x0132
  F18 = 307,          // 0x0133
  F19 = 308,          // 0x0134
  F20 = 309,          // 0x0135
  F21 = 310,          // 0x0136
  F22 = 311,          // 0x0137
  F23 = 312,          // 0x0138
  F24 = 313,          // 0x0139
  F25 = 314,          // 0x013A

  // Keypad keys
  Kp0 = 320,         // 0x0140
  Kp1 = 321,         // 0x0141
  Kp2 = 322,         // 0x0142
  Kp3 = 323,         // 0x0143
  Kp4 = 324,         // 0x0144
  Kp5 = 325,         // 0x0145
  Kp6 = 326,         // 0x0146
  Kp7 = 327,         // 0x0147
  Kp8 = 328,         // 0x0148
  Kp9 = 329,         // 0x0149
  KpDecimal = 330,   // 0x014A
  KpDivide = 331,    // 0x014B
  KpMultiply = 332,  // 0x014C
  KpSubtract = 333,  // 0x014D
  KpAdd = 334,       // 0x014E
  KpEnter = 335,     // 0x014F
  KpEqual = 336,     // 0x0150

  // Modifier keys
  LeftShift = 340,     // 0x0154
  LeftControl = 341,   // 0x0155
  LeftAlt = 342,       // 0x0156
  LeftSuper = 343,     // 0x0157
  RightShift = 344,    // 0x0158
  RightControl = 345,  // 0x0159
  RightAlt = 346,      // 0x015A
  RightSuper = 347,    // 0x015B
  Menu = 348,          // 0x015C
};

// Action enum (bits 16-19) - strongly typed
enum class Action : u32 {
  Press = 0x00010000,    // bit 16
  Release = 0x00020000,  // bit 17
  Repeat = 0x00040000,   // bit 18
};

// Modifier enum (bits 20-27) - flag enum for bitwise operations
enum class Mod : u32 {
  None = 0x00000000,
  Shift = 0x00100000,     // bit 20
  Control = 0x00200000,   // bit 21
  Alt = 0x00400000,       // bit 22
  Super = 0x00800000,     // bit 23
  CapsLock = 0x01000000,  // bit 24
  NumLock = 0x02000000,   // bit 25
};

// MouseButton enum - similar pattern to Key (uses bits 0-15)
enum class MouseBtn : u32 {
  Button1 = 0x0001,
  Button2 = 0x0002,
  Button3 = 0x0003,
  Button4 = 0x0004,
  Button5 = 0x0005,
  Button6 = 0x0006,
  Button7 = 0x0007,
  Button8 = 0x0008,
  Left = Button1,
  Right = Button2,
  Middle = Button3,
};

// Subscription handle - opaque identifier for unsubscribing
struct SubscriptionHandle {
  u64 id;
  bool operator==(const SubscriptionHandle&) const = default;
};

// Bitwise OR operator overloads for fluent token construction API
constexpr u32 operator|(Key key, Action action) noexcept {
  return static_cast<u32>(key) | static_cast<u32>(action);
}

constexpr u32 operator|(u32 token, Mod mod) noexcept {
  return token | static_cast<u32>(mod);
}

constexpr u32 operator|(Mod lhs, Mod rhs) noexcept {
  return static_cast<u32>(lhs) | static_cast<u32>(rhs);
}

constexpr u32 operator|(MouseBtn button, Action action) noexcept {
  return static_cast<u32>(button) | static_cast<u32>(action);
}

// Helper functions to extract components from token
inline constexpr Key extractKey(u32 token) noexcept {
  return static_cast<Key>(token & 0x0000FFFF);
}

inline constexpr MouseBtn extractMouseButton(u32 token) noexcept {
  return static_cast<MouseBtn>(token & 0x0000FFFF);
}

inline constexpr Action extractAction(u32 token) noexcept {
  return static_cast<Action>(token & 0x000F0000);
}

inline constexpr Mod extractMods(u32 token) noexcept {
  return static_cast<Mod>(token & 0x0FF00000);
}

// Conversion functions between old (InputKey/InputMod/MouseButton) and new
// enums
inline Key toKey(InputKey inputKey) noexcept {
  return static_cast<Key>(inputKey);
}

inline InputKey fromKey(Key key) noexcept { return static_cast<InputKey>(key); }

inline Action toAction(InputKey inputAction) noexcept {
  // Map old action values to new bit positions
  // GLFW: PRESS=1, RELEASE=0, REPEAT=2
  // We need to convert to bit flags
  if (inputAction == ACTION_PRESS) return Action::Press;
  if (inputAction == ACTION_RELEASE) return Action::Release;
  if (inputAction == ACTION_REPEAT) return Action::Repeat;
  return Action::Press;  // fallback
}

inline InputKey fromAction(Action action) noexcept {
  // Convert first set bit back to GLFW action value
  u32 actionBits = static_cast<u32>(action);
  if (actionBits & static_cast<u32>(Action::Press)) return ACTION_PRESS;
  if (actionBits & static_cast<u32>(Action::Release)) return ACTION_RELEASE;
  if (actionBits & static_cast<u32>(Action::Repeat)) return ACTION_REPEAT;
  return ACTION_PRESS;  // fallback
}

inline Mod toMod(InputMod inputMod) noexcept {
  // Map old modifier bit flags to new bit positions
  // GLFW mods are in low bits, we need to shift to bits 20-27
  u32 result = 0;
  if (inputMod & MOD_SHIFT) result |= static_cast<u32>(Mod::Shift);
  if (inputMod & MOD_CONTROL) result |= static_cast<u32>(Mod::Control);
  if (inputMod & MOD_ALT) result |= static_cast<u32>(Mod::Alt);
  if (inputMod & MOD_SUPER) result |= static_cast<u32>(Mod::Super);
  if (inputMod & MOD_CAPS_LOCK) result |= static_cast<u32>(Mod::CapsLock);
  if (inputMod & MOD_NUM_LOCK) result |= static_cast<u32>(Mod::NumLock);
  return static_cast<Mod>(result);
}

inline InputMod fromMod(Mod mod) noexcept {
  // Convert back from our bit positions to GLFW positions
  u32 modBits = static_cast<u32>(mod);
  InputMod result = 0;
  if (modBits & static_cast<u32>(Mod::Shift)) result |= MOD_SHIFT;
  if (modBits & static_cast<u32>(Mod::Control)) result |= MOD_CONTROL;
  if (modBits & static_cast<u32>(Mod::Alt)) result |= MOD_ALT;
  if (modBits & static_cast<u32>(Mod::Super)) result |= MOD_SUPER;
  if (modBits & static_cast<u32>(Mod::CapsLock)) result |= MOD_CAPS_LOCK;
  if (modBits & static_cast<u32>(Mod::NumLock)) result |= MOD_NUM_LOCK;
  return result;
}

inline MouseBtn toMouseButton(MouseButton button) noexcept {
  return static_cast<MouseBtn>(button);
}

inline MouseButton fromMouseButton(MouseBtn button) noexcept {
  return static_cast<MouseButton>(button);
}

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_TOKEN_H_
