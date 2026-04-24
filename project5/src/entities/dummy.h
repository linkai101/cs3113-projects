#pragma once
#include "entities/entity.h"
#include "assets.h"
#include <vector>

class Dummy : public Entity {
public:
  Dummy(Vector2 spawnPosition, Assets& assets, float maxHealth = 100.0f);

  void update(float deltaTime) override;

  void render() const override;

  void takeDamage(float amount);

private:
  struct DamageIndicator {
    float amount;
    float timer;
    Vector2 offset;
  };

  enum class State { ALIVE, DYING, DEAD };

  static Animator buildAnimator(Spritesheet* sheet);

  void respawn();

  State state = State::ALIVE;
  float health;
  float maxHealth;
  float damageFlashTimer = 0.0f;
  float deadTimer = 0.0f;
  std::vector<DamageIndicator> damageIndicators;

  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
  static constexpr float DAMAGE_INDICATOR_DURATION = 1.0f;
  static constexpr float DEAD_DURATION = 2.0f;
  static constexpr Vector2 SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {40, 60};
};
