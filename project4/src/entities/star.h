#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Star : public Entity {
public:
  Star(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void resolveCollisions(std::vector<Entity*> entities) override;

private:
  Sound& tapSound;

  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
};