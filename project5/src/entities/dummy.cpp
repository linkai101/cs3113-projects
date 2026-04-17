#include "entities/dummy.h"
#include "raylib.h"

Dummy::Dummy(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(&assets.playerSheet))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, true);
  playAnimation("idle-side");
}

void Dummy::update(float deltaTime) {
  if (damageFlashTimer > 0.0f) {
    damageFlashTimer -= deltaTime;
    if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
  }

  Entity::update(deltaTime);
}

void Dummy::render() const {
  Entity::render();

  if (damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    BeginBlendMode(BLEND_ADDITIVE);
    animator->render(position, Fade(WHITE, alpha));
    EndBlendMode();
  }
}

void Dummy::takeDamage(float amount) {
  // TODO: apply damage to health when health is implemented
  damageFlashTimer = DAMAGE_FLASH_DURATION;
}

Animator Dummy::buildAnimator(Spritesheet* sheet) {
  Animator anim(sheet, SIZE, Vector2{SIZE.x / 2, SIZE.y});

  anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 3, true});
  anim.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 3, true});
  anim.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 3, true});
  anim.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 3, true});
  anim.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 3, true});
  anim.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 3, true});
  anim.addAnimation("attack-side", Animator::Animation{"attack-side", {54, 55, 56}, 3, false});
  anim.addAnimation("attack-down", Animator::Animation{"attack-down", {60, 61, 62}, 3, false});
  anim.addAnimation("attack-up", Animator::Animation{"attack-up", {66, 67, 68}, 3, false});
  anim.addAnimation("die", Animator::Animation{"die", {72, 73, 74, 75, 76, 77}, 3, false});

  return anim;
}
