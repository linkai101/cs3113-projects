#pragma once
#include "items/item.h"

class Bandage : public Item {
public:
  Bandage(Vector2 position, Assets& assets);

  static constexpr float HEAL_AMOUNT = 20.0f;

private:
  static constexpr Vector2 RENDER_SIZE = {25, 35};
};