#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"

class GhoulAxe : public Entity {
public:
  GhoulAxe(Vector2 position, float angleRad, Assets& assets);

  void update(float deltaTime) override;

  float getDamage() const { return DAMAGE; }

  bool isExpired() const { return distanceTraveled >= MAX_RANGE; }

private:
  Vector2 velocity;
  float distanceTraveled = 0.0f;

  static Animator buildAnimator(Spritesheet* sheet);

  static constexpr Vector2 RENDER_SIZE = {56.0f, 56.0f};
  static constexpr float SPEED = 400.0f;
  static constexpr float MAX_RANGE = 600.0f;
  static constexpr float DAMAGE = 33.0f;
};
