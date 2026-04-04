#include "entities/star.h"

Star::Star(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(assets)),
  tapSound(assets.tapSound)
{
  enablePhysics(
    Vector2{50, 70}, // colliderSize
    Vector2{-25, -30}, // colliderOffset
    false
  );
  playAnimation("idle");
}

void Star::update(float deltaTime) {
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;
    pb.acceleration.y = GRAVITY_ACCELERATION;
  }

  Entity::update(deltaTime);
}

void Star::resolveCollisions(std::vector<Entity*> entities) {
  Entity::resolveCollisions(entities);
}

Animator Star::buildAnimator(Assets& assets) {
  Animator starAnimator = Animator(
    &assets.starSheet,
    Vector2{102, 90}, // size
    Vector2{51, 45} // origin
  );
  starAnimator.addAnimation("idle", Animator::Animation{"idle", {0, 1, 2, 3, 4, 5, 6, 7}, 10, true});
  starAnimator.addAnimation("run", Animator::Animation{"run", {8, 9, 10, 11, 12, 13}, 10, true});
  starAnimator.addAnimation("jump", Animator::Animation{"jump", {16, 17, 18}, 10, false});
  starAnimator.addAnimation("fall", Animator::Animation{"fall", {24}, 10, true});
  starAnimator.addAnimation("ground", Animator::Animation{"ground", {32, 33}, 10, false});
  starAnimator.addAnimation("anticipation", Animator::Animation{"anticipation", {40, 41, 42}, 10, false});
  starAnimator.addAnimation("attack", Animator::Animation{"attack", {48, 49, 50, 51}, 10, false});
  starAnimator.addAnimation("hit", Animator::Animation{"hit", {56, 57, 58, 59}, 10, false});
  // starAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", {64, 65, 66, 67}, 10, false});
  // starAnimator.addAnimation("dead-ground", Animator::Animation{"dead-ground", {72, 73, 74, 75}, 10, false});
  starAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", {64, 65, 66, 67, 72, 73, 74, 75}, 10, false});

  return starAnimator;
}