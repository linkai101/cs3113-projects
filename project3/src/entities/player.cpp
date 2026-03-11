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

void Player::update(float deltaTime) {
  if (!physicsBody) return;

  std::string currentAnimation = animator->getCurrentAnimation();
  PhysicsBody& pb = *physicsBody;

  // Apply gravity/boost acceleration
  pb.acceleration.y = boosting ? -BOOST_Y_ACCELERATION : GRAVITY_ACCELERATION;

  if (pb.isGrounded) { // Grounded logic
    jumping = false;
    boosting = false;
    canBoost = false;

    // Animation
    if (movingLeft || movingRight) {
      if (currentAnimation != "standing_run") {
        playAnimation("standing_run");
      }
    } else {
      if (currentAnimation != "standing_idle") {
        playAnimation("standing_idle");
      }
    }

    // Ground horizontal movement
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
        canBoost = false;
        jumping = true;
        pb.velocity.y = -JUMP_INITIAL_VELOCITY;

        playAnimation("standing_jump");
      }
    } else {
      canJump = true; // Allow jumping when grounded after movingUp is released
    }

  } else { // Mid-air logic
    if (boosting) { // Boosting
      // Animation
      if (movingLeft || movingRight) {
        if (currentAnimation != "flying") {
          playAnimation("flying");
        }
      } else {
        if (currentAnimation != "flying_no_movement") {
          playAnimation("flying_no_movement");
        }
      }

      // Jetpack mid-air horizontal movement
      if (movingLeft && !movingRight) { // LEFT
        pb.acceleration.x = -BOOST_X_ACCELERATION;
        pb.velocity.x = std::max(pb.velocity.x, -BOOST_X_MAX_VELOCITY); // cap velocity
        animator->setFlipX(true);
      } else if (movingRight && !movingLeft) { // RIGHT
        pb.acceleration.x = BOOST_X_ACCELERATION;
        pb.velocity.x = std::min(pb.velocity.x, BOOST_X_MAX_VELOCITY); // cap velocity
        animator->setFlipX(false);
      } else { // NO HORIZONTAL MOVEMENT
        pb.acceleration.x = 0;
      }

      // Stop boosting if movingUp is released
      if (!movingUp) {
        boosting = false;
      }
    } else { // Not boosting
      if (currentAnimation != "flying_die" && currentAnimation != "standing_jump") {
        playAnimation("flying_die");
      }

      // Non-jetpack mid-air horizontal movement
      if (movingLeft && !movingRight) {
        if (jumping) pb.velocity.x = -GROUND_VELOCITY;
        else pb.acceleration.x = -GLIDE_X_ACCELERATION;

        animator->setFlipX(true); // change direction
      } else if (movingRight && !movingLeft) {
        if (jumping) pb.velocity.x = GROUND_VELOCITY;
        else pb.acceleration.x = GLIDE_X_ACCELERATION;

        animator->setFlipX(false); // change direction
      } else {
        if (jumping) pb.velocity.x *= 0.95f;
        else pb.acceleration.x = 0;
      }

      // Activate jetpack if movingUp is pressed
      if (movingUp) {
        if (canBoost) {
          boosting = true;
          jumping = false;
        }
      } else {
        canBoost = true; // Allow boosting mid-air after movingUp is released
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

void Player::move(bool left, bool right, bool up) {
  movingLeft = left;
  movingRight = right;
  movingUp = up;
}