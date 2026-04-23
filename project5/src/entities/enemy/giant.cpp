#include "entities/enemy/giant.h"
#include "entities/player.h"
#include "utils/color.h"
#include "raylib.h"
#include <cmath>

Giant::Giant(Vector2 spawnPosition, Assets& assets) :
  Enemy(spawnPosition, buildAnimator(&assets.giantSheet))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");
}

void Giant::update(float deltaTime) {
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
            attackCooldownTimer = isAttack2 ? ATTACK2_COOLDOWN : ATTACK_COOLDOWN;
            isAttacking = false;
            isAttack2 = false;
            std::string idleAnim =
              (facingDirection == Direction::DOWN) ? "idle-down"
              : (facingDirection == Direction::UP) ? "idle-up"
              : "idle-side";
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
            float attackDist = isAttack2 ? ATTACK2_DISTANCE : ATTACK_DISTANCE;
            if (dist <= attackDist) target->takeDamage(isAttack2 ? ATTACK2_DAMAGE : ATTACK_DAMAGE);
            if (facingDirection == Direction::DOWN) {
              playAnimation(isAttack2 ? "attack2-down" : "attack1-down");
            } else if (facingDirection == Direction::UP) {
              playAnimation(isAttack2 ? "attack2-up" : "attack1-up");
            } else {
              playAnimation(isAttack2 ? "attack2-side" : "attack1-side");
            }
          }
        } else if (
          attackCooldownTimer <= 0.0f &&
          ((attack1Count % 3 == 2 && dist <= ATTACK2_DISTANCE) || (attack1Count % 3 != 2 && dist <= ATTACK_DISTANCE))
        ) {
          // Commit to attack, begin wind up
          attack1Count++;
          isAttack2 = (attack1Count % 3 == 0);
          isWindingUp = true;
          windUpTimer = isAttack2 ? ATTACK2_WIND_UP_DURATION : WIND_UP_DURATION;
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

void Giant::render() const {
  Entity::render();

  if (state != State::DEAD && damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    animator->render(position, ColorFromHex("#AA4A44", 0.5f * alpha));
  }

}

void Giant::takeDamage(float amount) {
  if (state != State::ALIVE) return;

  damageFlashTimer = DAMAGE_FLASH_DURATION;
  health -= amount;

  if (health <= 0.0f) {
    health = 0.0f;
    state = State::DYING;
    isAttacking = false;
    isAttack2 = false;
    isWindingUp = false;
    playAnimation("die");
  }
}

Animator Giant::buildAnimator(Spritesheet* sheet) {
  Vector2 origin = Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y};

  Animator anim = Animator(sheet, RENDER_SIZE, origin);

  anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  anim.addAnimation("idle-down", Animator::Animation{"idle-down", {15, 16, 17, 18, 19, 20}, 10, true});
  anim.addAnimation("idle-up", Animator::Animation{"idle-up", {30, 31, 32, 33, 34, 35}, 10, true});
  anim.addAnimation("run-side", Animator::Animation{"run-side", {45, 46, 47, 48, 49, 50, 51, 52}, 10, true});
  anim.addAnimation("run-down", Animator::Animation{"run-down", {60, 61, 62, 63, 64, 65, 66, 67}, 10, true});
  anim.addAnimation("run-up", Animator::Animation{"run-up", {75, 76, 77, 78, 79, 80, 81, 82}, 10, true});
  anim.addAnimation("attack1-side", Animator::Animation{"attack1-side", {90, 91, 92, 93, 94, 95, 96, 97}, 10, false});
  anim.addAnimation("attack1-down", Animator::Animation{"attack1-down", {105, 106, 107, 108, 109, 110, 111, 112}, 10, false});
  anim.addAnimation("attack1-up", Animator::Animation{"attack1-up", {120, 121, 122, 123, 124, 125, 126, 127}, 10, false});
  anim.addAnimation("attack2-side", Animator::Animation{"attack2-side", {135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149}, 10, false});
  anim.addAnimation("attack2-down", Animator::Animation{"attack2-down", {150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164}, 10, false});
  anim.addAnimation("attack2-up", Animator::Animation{"attack2-up", {165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179}, 10, false});
  anim.addAnimation("die", Animator::Animation{"die", {180, 181, 182, 183, 184, 185, 186, 187}, 10, false});

  return anim;
}
