#pragma once
#include "items/item.h"
#include "items/equippable/gun.h"

class AmmoCrate : public Item {
public:
  AmmoCrate(Vector2 position, Gun::Type gunType, int amount, Assets& assets);

  Gun::Type getGunType() const { return gunType; }

  int getAmount() const { return amount; }

  static constexpr Vector2 RENDER_SIZE = {45, 35};

private:
  Gun::Type gunType;
  int amount;
};
