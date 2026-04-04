#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Tooth : public Entity {
public:
  Tooth(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void resolveCollisions(std::vector<Entity*> entities) override;

  void setTarget(Vector2 pos, bool playerStunned) { targetPosition = pos; this->playerStunned = playerStunned; }

  bool isAttacking() const { return state == State::ATTACKING; }

  bool isDead() const { return dead; }

  void kill();

private:
  enum class State { WANDERING, DETECTING, CHASING, ANTICIPATING, ATTACKING, COOLDOWN };

  bool dead = false;
  bool playerStunned = false;
  State state = State::WANDERING;
  bool wanderingRight = true;
  bool waiting = false;
  float waitTimer = 0.0f;
  float detectionTimer = 0.0f;
  float attackTimer = ATTACK_INTERVAL;
  float cooldownTimer = 0.0f;
  Vector2 targetPosition = {0, 0};

  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float WANDER_VELOCITY = 100.0f;
  static constexpr float CHASE_VELOCITY = 250.0f;
  static constexpr float DETECTION_RANGE = 400.0f;
  static constexpr float ATTACK_RANGE = 42.0f;
  static constexpr float ATTACK_INTERVAL = 0.5f;
  static constexpr float COOLDOWN_DURATION = 0.0f;
  static constexpr float WANDER_WAIT_DURATION = 1.0f;
  static constexpr float DETECTION_WAIT_DURATION = 1.0f;
};
