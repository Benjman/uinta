#include "uinta/scenes/city_game/pawn_system.h"

#include <algorithm>

#include "uinta/scenes/city_game/sim_time.h"

namespace uinta {

void updatePawnNeeds(Pawns& pawns, const PawnConfig& config,
                     f32 simHours) noexcept {
  for (size_t i = 0; i < pawns.count(); ++i) {
    PawnState state = pawns.states[i];

    switch (state) {
      case PawnState::Sleeping:
        // Energy recovers while sleeping
        pawns.energy[i] += config.energyRecoveryRate * simHours;
        // Hunger still rises, but slower
        pawns.hunger[i] += config.hungerRate * 0.5f * simHours;
        break;

      case PawnState::Working:
        // Earn money while working
        pawns.money[i] += config.hourlyWage * simHours;
        // Energy drains
        pawns.energy[i] -= config.energyDrainRate * simHours;
        // Hunger rises
        pawns.hunger[i] += config.hungerRate * simHours;
        break;

      case PawnState::Eating:
        // Instant consumption - reduce hunger, spend money
        pawns.hunger[i] -= config.mealHungerReduction;
        pawns.money[i] -= config.mealCost;
        // Energy still drains slightly
        pawns.energy[i] -= config.energyDrainRate * 0.5f * simHours;
        break;

      default:
        // Idle and Walking: normal drain
        pawns.energy[i] -= config.energyDrainRate * simHours;
        pawns.hunger[i] += config.hungerRate * simHours;
        break;
    }

    // Clamp values
    pawns.energy[i] = std::clamp(pawns.energy[i], 0.0f, 1.0f);
    pawns.hunger[i] = std::clamp(pawns.hunger[i], 0.0f, 1.0f);
  }
}

PawnState decidePawnState(const Pawns& pawns,
                          const IntersectionBuildings& buildings,
                          const SimTime& time, const PawnConfig& config,
                          size_t i) noexcept {
  f32 hunger = pawns.hunger[i];
  f32 energy = pawns.energy[i];
  PawnState currentState = pawns.states[i];
  BuildingId job = pawns.jobs[i];
  BuildingId home = pawns.homes[i];
  BuildingId currentBldg = pawns.currentBuilding[i];
  bool hasJob = job != NULL_BUILDING;
  bool isWorkHours = time.isWorkHours(config.workStartHour, config.workEndHour);
  bool atHome = currentBldg == home;
  bool atWork = currentBldg == job;

  // Priority 1: Critical energy - must sleep
  if (energy < config.criticalEnergyThreshold) {
    if (atHome) {
      return PawnState::Sleeping;
    }
    // Need to go home first
    return PawnState::Walking;
  }

  // Priority 2: Continue sleeping until rested
  if (currentState == PawnState::Sleeping && energy < config.wakeThreshold) {
    return PawnState::Sleeping;
  }

  // Priority 3: Critical hunger - must eat
  if (hunger > config.criticalHungerThreshold) {
    // Find if we're at a shop
    BuildingId shop =
        buildings.findNearestByType(BuildingType::Shop, pawns.positions[i]);
    if (currentBldg == shop) {
      return PawnState::Eating;
    }
    // Need to walk to shop
    return PawnState::Walking;
  }

  // Priority 4: Continue working if at work during work hours
  if (currentState == PawnState::Working && hasJob && isWorkHours && atWork) {
    return PawnState::Working;
  }

  // Priority 5: Go to work if work hours and have job
  if (hasJob && isWorkHours) {
    if (atWork) {
      return PawnState::Working;
    }
    // Need to walk to work
    return PawnState::Walking;
  }

  // Priority 6: Moderate hunger - go eat
  if (hunger > config.eatThreshold) {
    BuildingId shop =
        buildings.findNearestByType(BuildingType::Shop, pawns.positions[i]);
    if (currentBldg == shop) {
      return PawnState::Eating;
    }
    return PawnState::Walking;
  }

  // Priority 7: Low energy - go sleep
  if (energy < config.sleepThreshold) {
    if (atHome) {
      return PawnState::Sleeping;
    }
    return PawnState::Walking;
  }

  // Default: Idle (go home if not there)
  if (!atHome && currentBldg != NULL_BUILDING) {
    // If inside a building that's not home, go home
    return PawnState::Walking;
  }

  return PawnState::Idle;
}

// Determine where a pawn should go based on their needs
BuildingId determineDestination(const Pawns& pawns,
                                const IntersectionBuildings& buildings,
                                const SimTime& time, const PawnConfig& config,
                                size_t i) noexcept {
  f32 hunger = pawns.hunger[i];
  f32 energy = pawns.energy[i];
  BuildingId job = pawns.jobs[i];
  BuildingId home = pawns.homes[i];
  bool hasJob = job != NULL_BUILDING;
  bool isWorkHours = time.isWorkHours(config.workStartHour, config.workEndHour);

  // Critical energy -> home
  if (energy < config.criticalEnergyThreshold) {
    return home;
  }

  // Critical hunger -> shop
  if (hunger > config.criticalHungerThreshold) {
    return buildings.findNearestByType(BuildingType::Shop, pawns.positions[i]);
  }

  // Work hours -> job
  if (hasJob && isWorkHours) {
    return job;
  }

  // Moderate hunger -> shop
  if (hunger > config.eatThreshold) {
    return buildings.findNearestByType(BuildingType::Shop, pawns.positions[i]);
  }

  // Low energy -> home
  if (energy < config.sleepThreshold) {
    return home;
  }

  // Default -> home
  return home;
}

void transitionPawnState(Pawns& pawns, IntersectionBuildings& buildings,
                         const SimTime& time, const PawnConfig& config,
                         size_t i, PawnState newState) noexcept {
  PawnState oldState = pawns.states[i];

  // Exit old state
  if (oldState == PawnState::Working) {
    BuildingId job = pawns.jobs[i];
    if (job != NULL_BUILDING && buildings.currentOccupants[job] > 0) {
      buildings.currentOccupants[job]--;
    }
  }

  // Enter new state
  switch (newState) {
    case PawnState::Walking: {
      // Determine where we're going
      BuildingId dest = determineDestination(pawns, buildings, time, config, i);
      pawns.targetBuilding[i] = dest;
      if (dest != NULL_BUILDING) {
        pawns.destinations[i] = buildings.entryPoints[dest];
      }
      // Leave current building
      pawns.currentBuilding[i] = NULL_BUILDING;
      break;
    }

    case PawnState::Working: {
      // Only enter Working if actually at workplace
      BuildingId job = pawns.jobs[i];
      if (pawns.currentBuilding[i] == job) {
        buildings.currentOccupants[job]++;
      }
      break;
    }

    case PawnState::Eating: {
      // Eating is instant - handled in updatePawnNeeds
      break;
    }

    case PawnState::Sleeping: {
      // Only enter Sleeping if actually at home
      // (state change is valid, currentBuilding already set)
      break;
    }

    case PawnState::Idle:
      pawns.targetBuilding[i] = NULL_BUILDING;
      break;
  }

  pawns.states[i] = newState;
}

void pawnDecisionSystem(Pawns& pawns, IntersectionBuildings& buildings,
                        const SimTime& time, const PawnConfig& config,
                        f32 dt) noexcept {
  // Calculate simulation time elapsed
  f32 simHours = dt * time.simHoursPerRealSecond();

  // Update needs for all pawns
  updatePawnNeeds(pawns, config, simHours);

  // Make decisions for each pawn
  for (size_t i = 0; i < pawns.count(); ++i) {
    PawnState currentState = pawns.states[i];

    // Determine if pawn should make a new decision
    bool shouldDecide = false;

    switch (currentState) {
      case PawnState::Idle:
        // Always reconsider when idle
        shouldDecide = true;
        break;

      case PawnState::Walking:
        // Don't interrupt walking - movement system handles arrival
        shouldDecide = false;
        break;

      case PawnState::Eating:
        // Eating is instant, transition out immediately
        shouldDecide = true;
        break;

      case PawnState::Sleeping:
        // Check if should wake up
        if (pawns.energy[i] >= config.wakeThreshold) {
          shouldDecide = true;
        }
        break;

      case PawnState::Working:
        // Check if should leave work
        if (!time.isWorkHours(config.workStartHour, config.workEndHour) ||
            pawns.hunger[i] > config.criticalHungerThreshold ||
            pawns.energy[i] < config.criticalEnergyThreshold) {
          shouldDecide = true;
        }
        break;
    }

    if (shouldDecide) {
      PawnState newState = decidePawnState(pawns, buildings, time, config, i);

      if (newState != currentState) {
        transitionPawnState(pawns, buildings, time, config, i, newState);
      }
    }
  }
}

}  // namespace uinta
