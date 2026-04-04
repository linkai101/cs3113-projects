#include "entities/player.h"
#include "utils/log.h"
#include <algorithm>

Player::Player(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(assets))
{
  enablePhysics(
    Vector2{ 50, 70 }, // colliderSize
    Vector2{ -25, -35 }, // colliderOffset
    false
  );
  playAnimation("idle");
}

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

    if (stunned)  {
      // Decelerate knockback
      float decel = HIT_KNOCKBACK_DECEL_X * deltaTime;
      if (pb.velocity.x > 0) pb.velocity.x = std::max(0.0f, pb.velocity.x - decel);
      else pb.velocity.x = std::min(0.0f, pb.velocity.x + decel);

      // Count down stun timer
      stunTimer -= deltaTime;
      if (stunTimer <= 0.0f) {
        stunned = false;
      }
    } else {
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
  }

  Entity::update(deltaTime);
}

void Player::hit() {
  playAnimation("dead-hit");
  getPhysicsBody()->velocity.x = HIT_KNOCKBACK_SPEED_X * (getFlipX() ? 1.0f : -1.0f);
  getPhysicsBody()->velocity.y = HIT_KNOCKBACK_VELOCITY_Y;

  stunned = true;
  stunTimer = STUNNED_DURATION;
}

Animator Player::buildAnimator(Assets& assets) {
  Animator playerAnimator = Animator(
    &assets.captainSheet,
    Vector2{ 192, 120 }, // size
    Vector2{ 96, 60 } // origin
  );
  playerAnimator.addAnimation("idle", Animator::Animation{"idle", { 0, 1, 2, 3, 4 }, 10, true});
  playerAnimator.addAnimation("run", Animator::Animation{"run", { 6, 7, 8, 9, 10, 11 }, 10, true});
  playerAnimator.addAnimation("jump", Animator::Animation{"jump", { 12, 13, 14 }, 10, false});
  playerAnimator.addAnimation("fall", Animator::Animation{"fall", { 18 }, 10, true});
  playerAnimator.addAnimation("ground", Animator::Animation{"ground", { 24, 25 }, 10, false});
  playerAnimator.addAnimation("hit", Animator::Animation{"hit", { 30, 31, 32, 33 }, 10, false});
  // playerAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", { 36, 37, 38, 39 }, 10, false});
  // playerAnimator.addAnimation("dead-ground", Animator::Animation{"dead-ground", { 42, 43, 44, 45 }, 10, false});
  playerAnimator.addAnimation("dead-hit", Animator::Animation{"dead", { 36, 37, 38, 39, 42, 43, 44, 45 }, 10, false});
  return playerAnimator;
}