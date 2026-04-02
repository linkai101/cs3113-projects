#pragma once
#include "raylib.h"

struct PhysicsBody {
  Vector2 colliderSize;
  Vector2 colliderOffset = {0, 0};
  
  bool isStatic = false; // true if the body doesn't move but can be collided with, false otherwise
  Vector2 velocity = {0, 0};
  Vector2 acceleration = {0, 0};
  bool isGrounded = false;
  
  Rectangle getCollider(Vector2 position) const;

  void update(float deltaTime);
};