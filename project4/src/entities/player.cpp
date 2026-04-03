#include "player.h"
#include "utils/log.h"

#include <algorithm>

Player::Player(
  Vector2 spawnPosition,
  Animator animator
) :
  Entity(
    spawnPosition,
    animator
  )
{}

void Player::processInput() {
  bool left = IsKeyDown(KEY_A);
  bool right = IsKeyDown(KEY_D);
  bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE);

  movingLeft = left;
  movingRight = right;
  movingUp = up;
}

void Player::update(float deltaTime) {
  if (physicsBody.has_value()) {
    std::string currentAnimation = animator->getCurrentAnimation();
    PhysicsBody& pb = *physicsBody;

    // Apply gravity acceleration
    pb.acceleration.y = GRAVITY_ACCELERATION;

    if (pb.isGrounded) { // Grounded logic
      jumping = false;

      // Animations
      if (movingLeft || movingRight) {
        if (currentAnimation != "run") {
          playAnimation("run");
        }
      } else {
        if (currentAnimation != "idle") {
          playAnimation("idle");
        }
      }

      // Horizontal movement (grounded)
      if (movingLeft && !movingRight) { // LEFT
        pb.velocity.x = -GROUND_VELOCITY;
        pb.acceleration.x = 0;
        animator->setFlipX(true);
      } else if (movingRight && !movingLeft) { // RIGHT
        pb.velocity.x = GROUND_VELOCITY;
        pb.acceleration.x = 0;
        animator->setFlipX(false);
      } else { // NO HORIZONTAL MOVEMENT
        pb.velocity.x = 0;
        pb.acceleration.x = 0;
      }

      // Ground jump
      if (movingUp) {
        if (canJump) {
          canJump = false;
          jumping = true;
          pb.velocity.y = -JUMP_INITIAL_VELOCITY;
          playAnimation("jump");
        }
      } else {
        canJump = true; // Allow jumping when grounded again after movingUp is released
      }

    } else { // Mid-air logic
      // Animations
      if (!jumping && currentAnimation != "fall") {
        playAnimation("fall");
      }

      // Horizontal movement (mid-air)
      if (movingLeft && !movingRight) { // LEFT
        pb.velocity.x = -GROUND_VELOCITY;
        pb.acceleration.x = 0;
        animator->setFlipX(true);
      } else if (movingRight && !movingLeft) { // RIGHT
        pb.velocity.x = GROUND_VELOCITY;
        pb.acceleration.x = 0;
        animator->setFlipX(false);
      } else { // NO HORIZONTAL MOVEMENT
        pb.velocity.x = 0;
        pb.acceleration.x = 0;
      }
    }
  }

  Entity::update(deltaTime);
}

void Player::render() const {
  Entity::render();
}

void Player::playAnimation(const std::string& animationName) {
  animator->play(animationName);
}