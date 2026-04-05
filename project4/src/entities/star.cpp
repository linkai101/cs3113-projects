#include "entities/star.h"
#include <cmath>

Star::Star(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(assets)),
  tapSound(assets.tapSound)
{
  enablePhysics(
    Vector2{50, 50}, // colliderSize
    Vector2{-25, -10}, // colliderOffset
    false
  );
  playAnimation("idle");
}

void Star::update(float deltaTime) {
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;
    pb.acceleration.y = GRAVITY_ACCELERATION;

    if (dead) {
      pb.velocity.x = 0;
      if (animator->getCurrentAnimation() != "dead-hit") playAnimation("dead-hit");
      Entity::update(deltaTime);
      return;
    }

    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    bool playerIsRight = dx >= 0;
    bool inSightX = std::abs(dx) <= DETECTION_RANGE_X;
    bool inSightY = std::abs(dy) <= DETECTION_RANGE_Y;

    switch (state) {
      case State::IDLE:
        pb.velocity.x = 0;
        if (animator->getCurrentAnimation() != "idle") playAnimation("idle");
        if (inSightX && inSightY && !playerStunned) {
          state = State::CHASING;
          chaseTimer = CHASE_DURATION;
        }
        break;

      case State::CHASING:
        if (!inSightX || playerStunned) {
          state = State::IDLE;
          break;
        }
        chaseTimer -= deltaTime;
        if (chaseTimer <= 0) {
          state = State::ATTACKING;
          attackRollingRight = playerIsRight;
          animator->setFlipX(playerIsRight);
          playAnimation("attack");
          break;
        }
        pb.velocity.x = playerIsRight ? CHASE_VELOCITY : -CHASE_VELOCITY;
        if (animator->getCurrentAnimation() != "run") playAnimation("run");
        animator->setFlipX(playerIsRight);
        break;

      case State::ATTACKING:
        pb.velocity.x = attackRollingRight ? ATTACK_ROLL_VELOCITY : -ATTACK_ROLL_VELOCITY;
        if (!inSightX) {
          state = State::IDLE;
          pb.velocity.x = 0;
          playAnimation("idle");
        }
        break;
    }
  }

  Entity::update(deltaTime);
}

void Star::endAttack() {
  if (state == State::ATTACKING) {
    state = State::IDLE;
    physicsBody->velocity.x = 0;
    playAnimation("idle");
  }
}

void Star::kill() {
  dead = true;
  physicsBody->velocity.x = 0;
  PlaySound(tapSound);
}

void Star::resolveCollisions(std::vector<Entity*> entities) {
  float velocityXBefore = physicsBody.has_value() ? physicsBody->velocity.x : 0.0f;
  Entity::resolveCollisions(entities);

  // End attack roll when hitting a wall
  if (state == State::ATTACKING && velocityXBefore != 0 && physicsBody->velocity.x == 0) {
    state = State::IDLE;
    playAnimation("idle");
  }
}

Animator Star::buildAnimator(Assets& assets) {
  Animator starAnimator = Animator(
    &assets.starSheet,
    Vector2{102, 90}, // size
    Vector2{51, 45} // origin
  );
  starAnimator.addAnimation("idle", Animator::Animation{"idle", {0, 1, 2, 3, 4, 5, 6, 7}, 5, true});
  starAnimator.addAnimation("run", Animator::Animation{"run", {8, 9, 10, 11, 12, 13}, 5, true});
  starAnimator.addAnimation("jump", Animator::Animation{"jump", {16, 17, 18}, 5, false});
  starAnimator.addAnimation("fall", Animator::Animation{"fall", {24}, 5, true});
  starAnimator.addAnimation("ground", Animator::Animation{"ground", {32, 33}, 5, false});
  starAnimator.addAnimation("anticipation", Animator::Animation{"anticipation", {40, 41, 42}, 5, false});
  starAnimator.addAnimation("attack", Animator::Animation{"attack", {48, 49, 50, 51}, 5, true});
  starAnimator.addAnimation("hit", Animator::Animation{"hit", {56, 57, 58, 59}, 5, false});
  // starAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", {64, 65, 66, 67}, 10, false});
  // starAnimator.addAnimation("dead-ground", Animator::Animation{"dead-ground", {72, 73, 74, 75}, 10, false});
  starAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", {64, 65, 66, 67, 72, 73, 74, 75}, 10, false});

  return starAnimator;
}
