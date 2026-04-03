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

  void setJumping(bool jumping) { this->jumping = jumping; }
  
private:
  bool movingLeft = false;
  bool movingRight = false;
  bool movingUp = false;

  bool canJump = true;
  bool jumping = false;
  bool wasGrounded = true;

  Animator buildAnimator(Assets& assets);

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float GROUND_VELOCITY = 300.0f;
  static constexpr float JUMP_INITIAL_VELOCITY = 700.0f;
};