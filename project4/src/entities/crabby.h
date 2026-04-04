#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Crabby : public Entity {
public:
  Crabby(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void resolveCollisions(std::vector<Entity*> entities) override;

  void kill();

  void reverseDirection();

  bool isDead() const { return dead; }

private:
  bool dead = false;
  bool wanderingRight = true;
  bool waiting = false;
  float waitTimer = 0.0f;

  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float GROUND_VELOCITY = 200.0f;
  static constexpr float WAIT_DURATION = 2.0f;
};