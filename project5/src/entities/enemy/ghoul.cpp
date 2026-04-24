#include "entities/enemy/ghoul.h"
#include "entities/player.h"
#include "utils/color.h"
#include "raylib.h"
#include <cmath>

Ghoul::Ghoul(Vector2 spawnPosition, Assets& assets) :
  Enemy(spawnPosition, buildAnimator(&assets.ghoulSheet))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");
}

void Ghoul::update(float deltaTime) {
  switch (state) {
    case State::ALIVE: {
      // Update damage flash timer
      if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= deltaTime;
        if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
      }

      // Update attack cooldown timer
      if (attackCooldownTimer > 0.0f) {
        attackCooldownTimer -= deltaTime;
        if (attackCooldownTimer < 0.0f) attackCooldownTimer = 0.0f;
      }

      if (target && target->isDead()) {
        if (physicsBody.has_value()) physicsBody->velocity = {0, 0};
        std::string idleAnim =
          (facingDirection == Direction::DOWN) ? "idle-down" :
          (facingDirection == Direction::UP) ? "idle-up" : "idle-side";
        if (animator.has_value() && animator->getCurrentAnimation() != idleAnim)
          playAnimation(idleAnim);
      } else if (target) {
        float dx = target->getPosition().x - position.x;
        float dy = target->getPosition().y - position.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (isAttacking) {
          // Attack animation playing
          if (physicsBody.has_value()) physicsBody->velocity = {0, 0};
          if (animator.has_value() && animator->isAnimationDone()) {
            attackCooldownTimer = ATTACK_COOLDOWN;
            isAttacking = false;
            std::string idleAnim =
              (facingDirection == Direction::DOWN) ? "idle-down" :
              (facingDirection == Direction::UP) ? "idle-up" : "idle-side";
            playAnimation(idleAnim);
          }
        } else if (isWindingUp) {
          // Wind up before attacking
          if (physicsBody.has_value()) physicsBody->velocity = {0, 0};
          std::string idleAnim =
            (facingDirection == Direction::DOWN) ? "idle-down" :
            (facingDirection == Direction::UP) ? "idle-up" : "idle-side";
          if (animator.has_value() && animator->getCurrentAnimation() != idleAnim)
            playAnimation(idleAnim);
          windUpTimer -= deltaTime;
          if (windUpTimer <= 0.0f) {
            isWindingUp = false;
            isAttacking = true;
            pendingAxeThrow = true;
            pendingAxeOrigin = {position.x, position.y - COLLIDER_SIZE.y * 0.6f};
            pendingAxeAngle = atan2f(dy, dx);
            if (facingDirection == Direction::DOWN) {
              playAnimation("attack2-down");
            } else if (facingDirection == Direction::UP) {
              playAnimation("attack2-up");
            } else {
              playAnimation("attack2-side");
            }
          }
        } else if (dist <= ATTACK_DISTANCE && attackCooldownTimer <= 0.0f) {
          // Commit to attack, begin wind up
          isWindingUp = true;
          windUpTimer = WIND_UP_DURATION;
          if (physicsBody.has_value()) physicsBody->velocity = {0, 0};
          if (fabsf(dx) >= fabsf(dy)) {
            facingDirection = dx >= 0 ? Direction::RIGHT : Direction::LEFT;
            if (animator.has_value()) animator->setFlipX(dx < 0);
          } else if (dy > 0) {
            facingDirection = Direction::DOWN;
          } else {
            facingDirection = Direction::UP;
          }
        } else if (dist > ATTACK_DISTANCE && dist <= FOLLOW_DISTANCE) {
          // Begin following target
          float nx = dx / dist;
          float ny = dy / dist;
          if (physicsBody.has_value()) physicsBody->velocity = {nx * MOVEMENT_SPEED, ny * MOVEMENT_SPEED};
          std::string runAnim;
          if (fabsf(dx) >= fabsf(dy)) {
            facingDirection = dx >= 0 ? Direction::RIGHT : Direction::LEFT;
            if (animator.has_value()) animator->setFlipX(dx < 0);
            runAnim = "run-side";
          } else if (dy > 0) {
            facingDirection = Direction::DOWN;
            runAnim = "run-down";
          } else {
            facingDirection = Direction::UP;
            runAnim = "run-up";
          }
          if (animator.has_value() && animator->getCurrentAnimation() != runAnim)
            playAnimation(runAnim);
        } else {
          // Idle
          if (physicsBody.has_value()) physicsBody->velocity = {0, 0};
          std::string idleAnim =
            (facingDirection == Direction::DOWN) ? "idle-down" :
            (facingDirection == Direction::UP) ? "idle-up" : "idle-side";
          if (animator.has_value() && animator->getCurrentAnimation() != idleAnim)
            playAnimation(idleAnim);
        }
      }

      Entity::update(deltaTime);
      break;
    }
    case State::DYING:
      if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= deltaTime;
        if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
      }
      Entity::update(deltaTime);
      if (animator.has_value() && animator->isAnimationDone()) {
        state = State::DEAD;
        physicsBody = std::nullopt;
      }
      break;
    case State::DEAD:
      break;
  }
}

void Ghoul::render() const {
  Entity::render();

  if (state != State::DEAD && damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    animator->render(position, ColorFromHex("#AA4A44", 0.5f * alpha));
  }

}

void Ghoul::takeDamage(float amount) {
  if (state != State::ALIVE) return;

  damageFlashTimer = DAMAGE_FLASH_DURATION;
  health -= amount;

  if (health <= 0.0f) {
    health = 0.0f;
    state = State::DYING;
    isAttacking = false;
    isWindingUp = false;
    playAnimation("die");
  }
}

Animator Ghoul::buildAnimator(Spritesheet* sheet) {
  Vector2 origin = Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y};

  Animator anim = Animator(sheet, RENDER_SIZE, origin);

  anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  anim.addAnimation("idle-down", Animator::Animation{"idle-down", {9, 10, 11, 12, 13, 14}, 10, true});
  anim.addAnimation("idle-up", Animator::Animation{"idle-up", {18, 19, 20, 21, 22, 23}, 10, true});
  anim.addAnimation("run-side", Animator::Animation{"run-side", {27, 28, 29, 30, 31, 32, 33, 34}, 10, true});
  anim.addAnimation("run-down", Animator::Animation{"run-down", {36, 37, 38, 39, 40, 41, 42, 43}, 10, true});
  anim.addAnimation("run-up", Animator::Animation{"run-up", {45, 46, 47, 48, 49, 50, 51, 52}, 10, true});
  anim.addAnimation("attack1-side", Animator::Animation{"attack1-side", {54, 55, 56, 57, 58, 59, 60}, 10, false});
  anim.addAnimation("attack1-down", Animator::Animation{"attack1-down", {63, 64, 65, 66, 67, 68, 69}, 10, false});
  anim.addAnimation("attack1-up", Animator::Animation{"attack1-up", {72, 73, 74, 75, 76, 77, 78}, 10, false});
  anim.addAnimation("attack2-side", Animator::Animation{"attack2-side", {81, 82, 83, 84, 85, 86, 87, 88, 89}, 10, false});
  anim.addAnimation("attack2-down", Animator::Animation{"attack2-down", {90, 91, 92, 93, 94, 95, 96, 97, 98}, 10, false});
  anim.addAnimation("attack2-up", Animator::Animation{"attack2-up", {99, 100, 101, 102, 103, 104, 105, 106, 107}, 10, false});
  anim.addAnimation("die", Animator::Animation{"die", {108, 109, 110, 111, 112, 113}, 10, false});

  return anim;
}
