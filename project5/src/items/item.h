#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"

class Item {
public:
  Item() = default;

  virtual ~Item() = default;

  // Drop the item on the ground
  // void drop(Vector2 position);

private:
  bool isGrounded = false;
  
  // TODO: Entity to render when grounded
  // Entity object;
};