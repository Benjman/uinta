#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ECONOMY_SYSTEM_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ECONOMY_SYSTEM_H_

#include <cmath>

#include "./building_types.h"
#include "uinta/types.h"

namespace uinta {

class EconomySystem {
 public:
  explicit EconomySystem(f32 initialMoney = 0) noexcept : money_(initialMoney) {}

  void update(time_t dt, size_t houseCount, size_t factoryCount) noexcept;

  f32 money() const noexcept { return money_; }
  void setMoney(f32 money) noexcept { money_ = money; }

  bool canAfford(f32 cost) const noexcept { return money_ >= cost; }
  void deductMoney(f32 amount) noexcept { money_ -= amount; }
  void addMoney(f32 amount) noexcept { money_ += amount; }

  f32 buildingCost(BuildingType type, size_t currentCount) const noexcept;
  f32 incomePerSecond(size_t houseCount, size_t factoryCount) const noexcept;

  f32 houseMoney(size_t houseCount) const noexcept;
  f32 factoryMoney(size_t factoryCount) const noexcept;

 private:
  f32 money_ = 0;
  f32 moneyTimer_ = 0;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ECONOMY_SYSTEM_H_
