#include "uinta/scenes/city_game/economy_system.h"

namespace uinta {

void EconomySystem::update(time_t dt, size_t houseCount,
                           size_t factoryCount) noexcept {
  moneyTimer_ += dt;
  static constexpr auto MoneyUpdateHz = 0.1;
  if (moneyTimer_ < MoneyUpdateHz) {
    return;
  }

  const auto income = incomePerSecond(houseCount, factoryCount) * moneyTimer_;
  if (income > 0) {
    money_ += income;
  }

  moneyTimer_ = 0.0;
}

f32 EconomySystem::buildingCost(BuildingType type,
                                size_t currentCount) const noexcept {
  auto baseCost = initialBuildingCost(type);
  auto inflationRate = buildingInflationRate(type);
  return baseCost * std::pow(inflationRate, currentCount);
}

f32 EconomySystem::incomePerSecond(size_t houseCount,
                                   size_t factoryCount) const noexcept {
  return houseMoney(houseCount) + factoryMoney(factoryCount);
}

f32 EconomySystem::houseMoney(size_t houseCount) const noexcept {
  static constexpr auto AmountPerHouse = 0.1;
  return houseCount * AmountPerHouse;
}

f32 EconomySystem::factoryMoney(size_t factoryCount) const noexcept {
  static constexpr auto AmountPerFactory = 1.0;
  return factoryCount * AmountPerFactory;
}

}  // namespace uinta
