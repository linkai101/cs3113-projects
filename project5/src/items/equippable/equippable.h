#pragma once
#include "items/item.h"
#include "assets.h"

class Equippable : public Item {
public:
  Equippable() = default;

  virtual void update(float deltaTime) = 0;

  // Renders the item in the equipped position relative to the player
  virtual void render(Vector2 position) const = 0;
};