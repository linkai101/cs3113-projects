#include "entities/tooth.h"
#include <cmath>

Tooth::Tooth(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(assets))
{
  enablePhysics(
    Vector2{ 50, 50 }, // colliderSize
    Vector2{ -25, -12 }, // colliderOffset
    false
  );
  playAnimation("idle");
}

void Tooth::update(float deltaTime) {
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
    bool playerIsRight = dx >= 0;
    bool inRange = std::abs(dx) <= DETECTION_RANGE;

    switch (state) {
      case State::WANDERING:
        if (inRange && !playerStunned) {
          state = State::DETECTING;
          detectionTimer = DETECTION_WAIT_DURATION;
          pb.velocity.x = 0;
          if (animator->getCurrentAnimation() != "idle") playAnimation("idle");
          break;
        }
        if (waiting) {
          pb.velocity.x = 0;
          if (animator->getCurrentAnimation() != "idle") playAnimation("idle");
          waitTimer -= deltaTime;
          if (waitTimer <= 0) {
            waiting = false;
            wanderingRight = !wanderingRight;
            animator->setFlipX(wanderingRight);
          }
        } else {
          pb.velocity.x = wanderingRight ? WANDER_VELOCITY : -WANDER_VELOCITY;
          if (animator->getCurrentAnimation() != "run") playAnimation("run");
          animator->setFlipX(wanderingRight);
        }
        break;

      case State::DETECTING:
        pb.velocity.x = 0;
        if (!inRange || playerStunned) {
          state = State::WANDERING;
          break;
        }
        detectionTimer -= deltaTime;
        if (detectionTimer <= 0) {
          state = State::CHASING;
          attackTimer = ATTACK_INTERVAL;
        }
        break;

      case State::CHASING:
        if (!inRange || playerStunned) {
          state = State::WANDERING;
          waiting = false;
          break;
        }
        attackTimer -= deltaTime;
        if (attackTimer <= 0 && std::abs(dx) <= ATTACK_RANGE) {
          state = State::ANTICIPATING;
          pb.velocity.x = 0;
          playAnimation("anticipation");
          break;
        }
        pb.velocity.x = playerIsRight ? CHASE_VELOCITY : -CHASE_VELOCITY;
        if (animator->getCurrentAnimation() != "run") playAnimation("run");
        animator->setFlipX(playerIsRight);
        break;

      case State::ANTICIPATING:
        pb.velocity.x = 0;
        if (animator->isAnimationDone()) {
          state = State::ATTACKING;
          playAnimation("attack");
        }
        break;

      case State::ATTACKING:
        pb.velocity.x = 0;
        if (animator->isAnimationDone()) {
          state = State::COOLDOWN;
          cooldownTimer = COOLDOWN_DURATION;
          playAnimation("idle");
        }
        break;

      case State::COOLDOWN:
        pb.velocity.x = 0;
        cooldownTimer -= deltaTime;
        if (cooldownTimer <= 0) {
          state = State::CHASING;
          attackTimer = ATTACK_INTERVAL;
        }
        break;
    }
  }

  Entity::update(deltaTime);
}

void Tooth::resolveCollisions(std::vector<Entity*> entities) {
  float velocityXBefore = physicsBody.has_value() ? physicsBody->velocity.x : 0.0f;
  Entity::resolveCollisions(entities);

  // Reverse wander direction when hitting a wall
  if (state == State::WANDERING && !waiting && velocityXBefore != 0 && physicsBody->velocity.x == 0) {
    waiting = true;
    waitTimer = WANDER_WAIT_DURATION;
  }
}

void Tooth::kill() {
  dead = true;
  physicsBody->velocity.x = 0;
}

Animator Tooth::buildAnimator(Assets& assets) {
  Animator toothAnimator = Animator(
    &assets.toothSheet,
    Vector2{ 102, 90 }, // size
    Vector2{ 51, 45 } // origin
  );
  toothAnimator.addAnimation("idle", Animator::Animation{"idle", { 0, 1, 2, 3, 4, 5, 6, 7 }, 10, true});
  toothAnimator.addAnimation("run", Animator::Animation{"run", { 8, 9, 10, 11, 12, 13 }, 10, true});
  toothAnimator.addAnimation("jump", Animator::Animation{"jump", { 16, 17, 18 }, 10, false});
  toothAnimator.addAnimation("fall", Animator::Animation{"fall", { 24 }, 10, true});
  toothAnimator.addAnimation("ground", Animator::Animation{"ground", { 32, 33 }, 10, false});
  toothAnimator.addAnimation("anticipation", Animator::Animation{"anticipation", { 40, 41, 42 }, 10, false});
  toothAnimator.addAnimation("attack", Animator::Animation{"attack", { 48, 49, 50, 51 }, 10, false});
  toothAnimator.addAnimation("hit", Animator::Animation{"hit", { 56, 57, 58, 59 }, 10, false});
  toothAnimator.addAnimation("dead-hit", Animator::Animation{"dead-hit", { 64, 65, 66, 67, 72, 73, 74, 75 }, 10, false});
  return toothAnimator;
}
