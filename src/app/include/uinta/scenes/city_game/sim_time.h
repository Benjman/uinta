#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_SIM_TIME_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_SIM_TIME_H_

#include "uinta/types.h"

namespace uinta {

struct SimTime {
  f32 hour = 8.0f;       // 0.0 - 24.0, start at 8 AM
  u32 day = 0;           // Days elapsed
  f32 timeScale = 1.0f;  // Multiplier for simulation speed

  // Base rate: 1 simulation hour = 1 real minute = 60 real seconds
  // So: 1/60 sim-hours per real second at timeScale=1
  static constexpr f32 BASE_SIM_HOURS_PER_REAL_SECOND = 1.0f / 60.0f;

  inline f32 simHoursPerRealSecond() const noexcept {
    return BASE_SIM_HOURS_PER_REAL_SECOND * timeScale;
  }

  void advance(time_t realDt) noexcept {
    hour += static_cast<f32>(realDt) * simHoursPerRealSecond();
    while (hour >= 24.0f) {
      hour -= 24.0f;
      day++;
    }
  }

  // Get hour as integer (0-23)
  i32 hourInt() const noexcept { return static_cast<i32>(hour); }

  // Get minutes within current hour (0-59)
  i32 minuteInt() const noexcept {
    return static_cast<i32>((hour - static_cast<f32>(hourInt())) * 60.0f);
  }

  // Check if current time is within work hours
  bool isWorkHours(f32 startHour, f32 endHour) const noexcept {
    return hour >= startHour && hour < endHour;
  }
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_SIM_TIME_H_
