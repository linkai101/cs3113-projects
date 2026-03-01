#include "ball.h"
#include "entity.h"

Ball::Ball(Vector2 position) :
velocityX(0.0f),
velocityY(0.0f),
Entity(
  position,
  { 48.0f, 48.0f }, // size
  0.0f, // rotation
  WHITE, // tint
  "assets/textures/ball.png" // texture filepath
) {}

Ball::~Ball() {}

void Ball::update(float deltaTime) {
  // Apply gravity
  velocityY += GRAVITY * deltaTime;

  // Update position
  position.x += velocityX * deltaTime;
  position.y += velocityY * deltaTime;
}

void Ball::reflect(Vector2 surfaceNormal, float upwardAmplify, float maxUpwardSpeed) {
  // Ensure unit vector
  float len = sqrtf(surfaceNormal.x * surfaceNormal.x + surfaceNormal.y * surfaceNormal.y);
  surfaceNormal.x /= len;
  surfaceNormal.y /= len;

  // Only reflect if moving into the surface
  float dotProduct = velocityX * surfaceNormal.x + velocityY * surfaceNormal.y;
  if (dotProduct >= 0) return;

  velocityX -= 2.0f * dotProduct * surfaceNormal.x;
  velocityY -= 2.0f * dotProduct * surfaceNormal.y;

  // Amplify and clamp upward bounce (only used for paddle hits)
  if (upwardAmplify != 1.0f && velocityY < 0) {
    velocityY *= upwardAmplify;
    if (maxUpwardSpeed > 0.0f && velocityY < -maxUpwardSpeed) {
      velocityY = -maxUpwardSpeed;
    }
  }
}

void Ball::reset(Vector2 position) {
  velocityX = 0.0f;
  velocityY = 0.0f;
  this->position = position;
}