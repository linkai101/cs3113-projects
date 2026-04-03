#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Crabby : public Entity {
public:
  Crabby(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

private:
  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float GROUND_VELOCITY = 200.0f;
};