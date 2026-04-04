#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Player : public Entity {
public:
  Player(Vector2 spawnPosition, Assets& assets);

  void processInput();

  void update(float deltaTime) override;

  void hit(float hitSourceX);

  void setJumping(bool jumping) { this->jumping = jumping; }
  
  bool isStunned() const { return stunned; }
  bool setStunned(bool stunned) { this->stunned = stunned; }
  
private:
  Sound& jumpSound;
  Sound& hurtSound;

  bool movingLeft = false;
  bool movingRight = false;
  bool movingUp = false;

  bool canJump = true;
  bool jumping = false;
  bool wasGrounded = true;
  bool stunned = false; // after being hit, player is stunned for a short time
  float stunTimer = 0.0f;

  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float GROUND_VELOCITY = 300.0f;
  static constexpr float JUMP_INITIAL_VELOCITY = 700.0f;
  static constexpr float HIT_KNOCKBACK_SPEED_X = 250.0f; // direction determined by flipX
  static constexpr float HIT_KNOCKBACK_DECEL_X = 300.0f; // horizontal deceleration during stun
  static constexpr float HIT_KNOCKBACK_VELOCITY_Y = -350.0f;
  static constexpr float STUNNED_DURATION = 1.5f; // seconds
};