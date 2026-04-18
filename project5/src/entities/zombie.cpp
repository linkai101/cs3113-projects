#include "entities/zombie.h"
#include "raylib.h"

Zombie::Zombie(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(&assets.zombieSheet))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, true);
  playAnimation("idle-side");
}

void Zombie::update(float deltaTime) {
  switch (state) {
    case State::ALIVE:
      if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= deltaTime;
        if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
      }
      Entity::update(deltaTime);
      break;
    case State::DYING:
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

  if (state == State::ALIVE && damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    BeginBlendMode(BLEND_ADDITIVE);
    animator->render(position, Fade(WHITE, alpha));
    EndBlendMode();
  }

}

void Zombie::takeDamage(float amount) {
  if (state != State::ALIVE) return;

  damageFlashTimer = DAMAGE_FLASH_DURATION;
  health -= amount;

  if (health <= 0.0f) {
    health = 0.0f;
    state = State::DYING;
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