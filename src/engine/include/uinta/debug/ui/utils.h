#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_UTILS_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_UTILS_H_

#include <imgui.h>

#include <limits>
#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/time/time.h"
#include "uinta/types.h"

namespace uinta {

constexpr struct {
  f32 min = -std::numeric_limits<f32>::max();
  f32 zero = 0;
  f32 oneTenth = 0.1;
  f32 one = 1;
  f32 twenty = 20;
  f32 oneHundred = 100;
  f32 threeSixty = 360;
  f32 max = std::numeric_limits<f32>::max();
} Limits;

extern f32 UiSpacing;
extern f32 UiWidth;
extern f32 UiHalfWidth;
extern f32 UiOneThirdsWidth;
extern f32 UiTwoThirdsWidth;
extern f32 UiOneQuartersWidth;
extern f32 UiOneFifthsWidth;
extern f32 UiThreeQuartersWidth;

inline constexpr u8 OmitMiilliseconds = 1 << 1;
inline constexpr u8 OmitMicroseconds = 1 << 2;

inline std::string formatDuration(absl::Duration duration,
                                  u8 flags = 0) noexcept {
  auto total_seconds = absl::ToInt64Seconds(duration);
  auto nanoseconds = duration % absl::Seconds(1);

  auto seconds = total_seconds % 60;
  auto minutes = (total_seconds / 60) % 60;
  auto hours = (total_seconds / (60 * 60)) % 24;

  auto milliseconds = nanoseconds / absl::Nanoseconds(1000000);
  auto microseconds =
      (nanoseconds % absl::Nanoseconds(1000000)) / absl::Nanoseconds(1000);

  std::string result;

  if (hours) result = absl::StrCat(hours, "h ");
  if (minutes) result = absl::StrCat(result, minutes, "m ");
  result = absl::StrCat(result, absl::StrFormat("%d", seconds));
  if (!(flags & OmitMiilliseconds))
    result = absl::StrCat(result, absl::StrFormat(".%03d", milliseconds));
  if (!(flags & OmitMicroseconds))
    result = absl::StrCat(result, absl::StrFormat("%03d", microseconds));

  return absl::StrCat(result, "s");
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_UTILS_H_
