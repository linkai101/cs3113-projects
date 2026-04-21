#pragma once
#include "entities/enemy/enemy.h"
#include "assets.h"
#include <utility>

class Ghoul : public Enemy {
public:
  Ghoul(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void takeDamage(float amount) override;

  void setTarget(Player* p) override { target = p; }

  bool isDead() const override { return state == State::DEAD; }

  bool hasPendingAxe() const { return pendingAxeThrow; }
  std::pair<Vector2, float> consumePendingAxe() {
    pendingAxeThrow = false;
    return {pendingAxeOrigin, pendingAxeAngle};
  }

private:
  enum class State { ALIVE, DYING, DEAD };

  Direction facingDirection = Direction::RIGHT;

  static Animator buildAnimator(Spritesheet* sheet);

  State state = State::ALIVE;
  float health = MAX_HEALTH;
  float damageFlashTimer = 0.0f;

  Player* target = nullptr;
  bool isAttacking = false;
  bool isWindingUp = false;
  float windUpTimer = 0.0f;
  float attackCooldownTimer = 0.0f;

  bool pendingAxeThrow = false;
  Vector2 pendingAxeOrigin = {};
  float pendingAxeAngle = 0.0f;

  static constexpr Vector2 RENDER_SIZE = {135, 135};
  static constexpr Vector2 COLLIDER_SIZE = {40, 72};
  static constexpr float MOVEMENT_SPEED = 100.0f;
  static constexpr float MAX_HEALTH = 100.0f;
  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
  static constexpr float FOLLOW_DISTANCE = 1000.0f;

  static constexpr float ATTACK_DISTANCE = 300.0f;
  static constexpr float ATTACK_COOLDOWN = 1.5f;
  static constexpr float WIND_UP_DURATION = 0.3f;
};
