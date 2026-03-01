#include "player.h"
#include "entity.h"

Player::Player(
  Vector2 position,
  const float minX,
  const float maxX
) :
velocityX(0.0f),
angularVelocity(0.0f),
minX(minX),
maxX(maxX),
Entity(
  position,
  { 80.0f, 15.0f }, // size
  0.0f, // rotation
  WHITE, // tint
  "assets/textures/paddle.png" // texture filepath
) {}

Player::~Player() {}

void Player::update(float deltaTime) {
  // Update position
  position.x += velocityX * deltaTime;
  // Clamp position
  if (position.x < minX) {
    position.x = minX;
  } else if (position.x > maxX) {
    position.x = maxX;
  }
  
  // Update angular velocity
  if (velocityX < 0.0f) {
    angularVelocity = -ANGULAR_VEL;
  } else if (velocityX > 0.0f) {
    angularVelocity = ANGULAR_VEL;
  } else {
    // pull rotation back towards flat
    angularVelocity -= rotation * RETURN_SPRING * deltaTime;
    angularVelocity -= angularVelocity * RETURN_DAMPING * deltaTime;
  }

  // Update rotation
  rotation += angularVelocity * deltaTime;
  // Clamp rotation
  if (rotation < -MAX_ROTATION) {
    rotation = -MAX_ROTATION;
  } else if (rotation > MAX_ROTATION) {
    rotation = MAX_ROTATION;
  }
}

void Player::move(MoveDirection moveDirection) {
  switch (moveDirection) {
    case NONE:
      velocityX = 0.0f;
      break;
    case LEFT:
      velocityX = -VELOCITY;
      break;
    case RIGHT:
      velocityX = VELOCITY;
      break;
  }
}