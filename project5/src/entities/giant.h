#pragma once
#include "entities/enemy.h"
#include "assets.h"

class Giant : public Enemy {
public:
  Giant(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void takeDamage(float amount) override;

  void setTarget(Player* p) override { target = p; }

private:
  enum class State { ALIVE, DYING, DEAD };

  Direction facingDirection = Direction::RIGHT;

  static Animator buildAnimator(Spritesheet* sheet);

  State state = State::ALIVE;
  float health = MAX_HEALTH;
  float damageFlashTimer = 0.0f;

  Player* target = nullptr;
  bool isAttacking = false;
  float attackCooldownTimer = 0.0f;

  static constexpr Vector2 RENDER_SIZE = {150, 150};
  static constexpr Vector2 COLLIDER_SIZE = {60, 100};
  static constexpr float MOVEMENT_SPEED = 50.0f;
  static constexpr float MAX_HEALTH = 200.0f;
  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
  static constexpr float FOLLOW_DISTANCE = 700.0f;
  static constexpr float ATTACK_DISTANCE = 70.0f;
  static constexpr float ATTACK_DAMAGE = 25.0f;
  static constexpr float ATTACK_COOLDOWN = 2.0f;
};
