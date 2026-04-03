#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "components/animator.h"

class Player : public Entity {
public:
  Player(Vector2 spawnPosition, Animator animator);

  void processInput();

  void update(float deltaTime) override;

  void render() const override;

  void playAnimation(const std::string& animationName);

  void setJumping(bool jumping) { this->jumping = jumping; }
  
private:
  bool movingLeft = false;
  bool movingRight = false;
  bool movingUp = false;

  bool canJump = true;
  bool jumping = false;
  bool wasGrounded = true;

  static constexpr float GRAVITY_ACCELERATION = 2100.0f;
  static constexpr float GROUND_VELOCITY = 300.0f;
  static constexpr float JUMP_INITIAL_VELOCITY = 700.0f;
};