#pragma once
#include "entities/entity.h"
#include "assets.h"

class Player;

class Zombie : public Entity {
public:
  Zombie(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void takeDamage(float amount);

  void setTarget(Player* p) { target = p; }

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

  static constexpr Vector2 RENDER_SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {40, 60};
  static constexpr float MOVEMENT_SPEED = 75.0f;
  static constexpr float MAX_HEALTH = 50.0f;
  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
  static constexpr float FOLLOW_DISTANCE = 400.0f;
  static constexpr float ATTACK_DISTANCE = 70.0f;
  static constexpr float ATTACK_DAMAGE = 10.0f;
  static constexpr float ATTACK_COOLDOWN = 1.5f;
};
