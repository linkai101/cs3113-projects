#include "entities/enemy/zombie.h"
#include "entities/player.h"
#include "utils/color.h"
#include "raylib.h"
#include <cmath>

Zombie::Zombie(Vector2 spawnPosition, Assets& assets) :
  Enemy(spawnPosition, buildAnimator(&assets.zombieSheet))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");
}

void Zombie::update(float deltaTime) {
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
            if (dist <= ATTACK_DISTANCE) target->takeDamage(ATTACK_DAMAGE);
            if (facingDirection == Direction::DOWN) {
              playAnimation("attack-down");
            } else if (facingDirection == Direction::UP) {
              playAnimation("attack-up");
            } else {
              playAnimation("attack-side");
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

void Zombie::render() const {
  Entity::render();

  if (state != State::DEAD && damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    animator->render(position, ColorFromHex("#AA4A44", 0.5f * alpha));
  }

}

void Zombie::takeDamage(float amount) {
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

Animator Zombie::buildAnimator(Spritesheet* sheet) {
  Vector2 origin = Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y};

  Animator anim = Animator(sheet, RENDER_SIZE, origin);

  anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  anim.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 10, true});
  anim.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 10, true});
  anim.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
  anim.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
  anim.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
  anim.addAnimation("attack-side", Animator::Animation{"attack-side", {36, 37, 38}, 10, false});
  anim.addAnimation("attack-down", Animator::Animation{"attack-down", {42, 43, 44}, 10, false});
  anim.addAnimation("attack-up", Animator::Animation{"attack-up", {48, 49, 50}, 10, false});
  anim.addAnimation("die", Animator::Animation{"die", {54, 55, 56, 57, 58, 59}, 10, false});

  return anim;
}
