#include "entities/ghoul_axe.h"
#include <cmath>

GhoulAxe::GhoulAxe(Vector2 position, float angleRad, Assets& assets) :
  Entity(position, buildAnimator(&assets.ghoulAxeThrownSheet)),
  velocity({cosf(angleRad) * SPEED, sinf(angleRad) * SPEED})
{
  playAnimation("spin");
}

void GhoulAxe::update(float deltaTime) {
  float dx = velocity.x * deltaTime;
  float dy = velocity.y * deltaTime;
  position.x += dx;
  position.y += dy;
  distanceTraveled += sqrtf(dx * dx + dy * dy);
  Entity::update(deltaTime);
}

Animator GhoulAxe::buildAnimator(Spritesheet* sheet) {
  Vector2 origin = {RENDER_SIZE.x / 2, RENDER_SIZE.y / 2};
  Animator anim(sheet, RENDER_SIZE, origin);
  anim.addAnimation("spin", Animator::Animation{"spin", {0, 1, 2, 3, 4, 5, 6, 7}, 12, true});
  return anim;
}
