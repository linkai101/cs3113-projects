#include "entities/crabby.h"

Crabby::Crabby(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(assets))
{
  enablePhysics(
    Vector2{72, 48}, // colliderSize
    Vector2{-36, -10}, // colliderOffset
    false
  );
  playAnimation("idle");
}

void Crabby::update(float deltaTime) {
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;

    // Apply gravity acceleration
    pb.acceleration.y = GRAVITY_ACCELERATION;
  }

  Entity::update(deltaTime);
}

Animator Crabby::buildAnimator(Assets& assets) {
  Animator crabbyAnimator = Animator(
    &assets.crabbySheet,
    Vector2{ 216, 96 }, // size
    Vector2{ 108, 48 } // origin
  );
  crabbyAnimator.addAnimation("idle", Animator::Animation{"idle", { 0, 1, 2, 3, 4, 5, 6, 7, 8 }, 10, true});

  return crabbyAnimator;
}