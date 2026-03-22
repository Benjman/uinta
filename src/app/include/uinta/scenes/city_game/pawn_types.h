#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_TYPES_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_TYPES_H_

#include "uinta/types.h"

namespace uinta {

using PawnId = u32;
constexpr PawnId NULL_PAWN = ~0u;

enum class PawnState : u8 {
  Idle,      // At home, awake, not doing anything
  Walking,   // Moving toward destination
  Working,   // At workplace, earning money
  Eating,    // At shop, consuming food
  Sleeping,  // At home, restoring energy
};

inline const char* pawnStateName(PawnState state) noexcept {
  switch (state) {
    case PawnState::Idle:
      return "Idle";
    case PawnState::Walking:
      return "Walking";
    case PawnState::Working:
      return "Working";
    case PawnState::Eating:
      return "Eating";
    case PawnState::Sleeping:
      return "Sleeping";
  }
  return "Unknown";
}

struct PawnConfig {
  // Hunger
  f32 hungerRate = 0.05f;              // per sim-hour
  f32 eatThreshold = 0.6f;             // seek food when hunger > this
  f32 criticalHungerThreshold = 0.8f;  // interrupt work for food
  f32 mealCost = 10.0f;                // money spent per meal
  f32 mealHungerReduction = 0.5f;      // hunger reduced per meal

  // Energy
  f32 energyDrainRate = 0.08f;         // per sim-hour while awake
  f32 energyRecoveryRate = 0.15f;      // per sim-hour while sleeping
  f32 sleepThreshold = 0.2f;           // go home when energy < this
  f32 criticalEnergyThreshold = 0.1f;  // emergency sleep
  f32 wakeThreshold = 0.9f;            // wake up when energy > this

  // Work
  f32 hourlyWage = 15.0f;    // money per sim-hour worked
  f32 workStartHour = 9.0f;  // when to head to work
  f32 workEndHour = 17.0f;   // when to leave work

  // Movement
  f32 walkingSpeed = 2.0f;  // units per second
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_TYPES_H_
