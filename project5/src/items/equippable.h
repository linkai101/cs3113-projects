#pragma once
#include "raylib.h"
#include "items/item.h"
#include "assets.h"

class Equippable : public Item {
public:
  Equippable() : Item() {};

  virtual void update(float deltaTime) = 0;

  virtual void render(Vector2 position) const = 0;
  
private:
};