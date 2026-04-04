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
    pb.acceleration.y = GRAVITY_ACCELERATION;

    if (dead) { // Dead
      if (animator->getCurrentAnimation() != "dead-hit") playAnimation("dead-hit");
    } else if (waiting) { // Alive, waiting
      pb.velocity.x = 0;
      if (animator->getCurrentAnimation() != "idle") playAnimation("idle");
      waitTimer -= deltaTime;
      if (waitTimer <= 0) {
        waiting = false;
        wanderingRight = !wanderingRight;
        animator->setFlipX(wanderingRight);
      }
    } else { // Alive, wandering
      pb.velocity.x = wanderingRight ? GROUND_VELOCITY : -GROUND_VELOCITY;
      if (animator->getCurrentAnimation() != "run") playAnimation("run");
      animator->setFlipX(wanderingRight);
    }
  }

  Entity::update(deltaTime);
}

void Crabby::resolveCollisions(std::vector<Entity*> entities) {
  float velocityXBefore = physicsBody.has_value() ? physicsBody->velocity.x : 0.0f;
  Entity::resolveCollisions(entities);

  // Check if crabby hit a wall
  if (!waiting && velocityXBefore != 0 && physicsBody->velocity.x == 0) {
    waiting = true;
    waitTimer = WAIT_DURATION;
  }
}

void Crabby::reverseDirection() {
  waiting = false;
  wanderingRight = !wanderingRight;
  animator->setFlipX(wanderingRight);
}

void Crabby::kill() {
  dead = true;
  physicsBody->velocity.x = 0;
  // physicsBody = std::nullopt; // Remove physics body
}

Animator Crabby::buildAnimator(Assets& assets) {
  Animator crabbyAnimator = Animator(
    &assets.crabbySheet,
    Vector2{ 216, 96 }, // size
    Vector2{ 108, 48 } // origin
  );
  crabbyAnimator.addAnimation("idle", Animator::Animation{"idle", { 0, 1, 2, 3, 4, 5, 6, 7, 8 }, 10, true});
  crabbyAnimator.addAnimation("run", Animator::Animation{"run", { 9, 10, 11, 12, 13, 14 }, 10, true});
  crabbyAnimator.addAnimation("jump", Animator::Animation{"jump", { 18, 19, 20 }, 10, false});
  crabbyAnimator.addAnimation("fall", Animator::Animation{"fall", { 27 }, 10, true});
  crabbyAnimator.addAnimation("ground", Animator::Animation{"ground", { 36, 37 }, 10, false});
  crabbyAnimator.addAnimation("anticipation", Animator::Animation{"anticipation", { 45, 46, 47 }, 10, false});
  crabbyAnimator.addAnimation("attack", Animator::Animation{"attack", { 54, 55, 56, 57 }, 10, false});
  crabbyAnimator.addAnimation("hit", Animator::Animation{"hit", { 63, 64, 65, 66 }, 10, false});
  // crabbyAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", { 72, 73, 74, 75 }, 10, false});
  // crabbyAnimator.addAnimation("dead-ground", Animator::Animation{"dead-ground", { 81, 82, 83, 84 }, 10, false});
  crabbyAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", { 72, 73, 74, 75, 81, 82, 83, 84 }, 10, false});


  return crabbyAnimator;
}