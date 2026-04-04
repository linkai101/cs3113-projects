#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Tooth : public Entity {
public:
  Tooth(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

private:
  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
};