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

  void setTarget(Vector2 pos, bool playerStunned) { targetPosition = pos; this->playerStunned = playerStunned; }

  bool isAttacking() const { return state == State::ATTACKING; }

  bool isDead() const { return dead; }

  void kill();
  void endAttack();

private:
  Sound& tapSound;

  enum class State { IDLE, CHASING, ATTACKING };

  bool dead = false;
  bool playerStunned = false;
  State state = State::IDLE;
  bool attackRollingRight = true;
  float chaseTimer = 0.0f;
  Vector2 targetPosition = {0, 0};

  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float DETECTION_RANGE_X = 400.0f;
  static constexpr float DETECTION_RANGE_Y = 60.0f;
  static constexpr float CHASE_VELOCITY = 275.0f;
  static constexpr float CHASE_DURATION = 1.0f;
  static constexpr float ATTACK_ROLL_VELOCITY = 400.0f;
};
