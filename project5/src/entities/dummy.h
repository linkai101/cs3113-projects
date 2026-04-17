#pragma once
#include "entities/entity.h"
#include "assets.h"

class Dummy : public Entity {
public:
  Dummy(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;
  void render() const override;

  void takeDamage(float amount);

private:
  static Animator buildAnimator(Spritesheet* sheet);

  float damageFlashTimer = 0.0f;

  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
  static constexpr Vector2 SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {40, 60};
};
