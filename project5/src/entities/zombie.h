#pragma once
#include "entities/entity.h"
#include "assets.h"

class Zombie : public Entity {
public:
  Zombie(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void takeDamage(float amount);

private:
  enum class State { ALIVE, DYING, DEAD };

  Direction facingDirection = Direction::RIGHT;

  static Animator buildAnimator(Spritesheet* sheet);

  State state = State::ALIVE;
  float health = MAX_HEALTH;
  float damageFlashTimer = 0.0f;

  static constexpr Vector2 RENDER_SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {40, 60};
  static constexpr float MOVEMENT_SPEED = 150.0f;
  static constexpr float MAX_HEALTH = 100.0f;
  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
};