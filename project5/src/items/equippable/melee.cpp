#include "entities/player.h"
#include "items/equippable/melee.h"

Melee::Melee(Type type, Assets& assets) :
  Equippable(),
  type(type),
  animator(buildAnimator(assets, type)),
  properties(getProperties(type))
{}

void Melee::update(float deltaTime) {
  // Update animator
  // Relay player animator state to melee animator
  if (animator.getCurrentAnimation() != playerCurrentAnimation) animator.play(playerCurrentAnimation);
  animator.setFlipX(playerFlipX);
  animator.update(deltaTime);

  // Update cooldown timer
  if (cooldownTimer > 0.0f) cooldownTimer -= deltaTime;
}

void Melee::render(Vector2 position) const {
  animator.render(position);
}

void Melee::triggerStrike() {
  cooldownTimer = properties.cooldown;
}

std::optional<Rectangle> Melee::getHitRect() const {
  if (
    playerCurrentAnimation != "attack-side" &&
    playerCurrentAnimation != "attack-down" &&
    playerCurrentAnimation != "attack-up"
  ) return std::nullopt;

  // Frame 0 is the windup, frames 1+ are the active swing
  if (playerCurrentFrame == 0) return std::nullopt;

  float range = properties.range;
  float hitboxWidth = properties.hitboxWidth;

  if (playerCurrentAnimation == "attack-side" && playerFlipX) { // Left
    return Rectangle{playerPosition.x - Player::COLLIDER_SIZE.x / 2 - range, playerPosition.y - Player::COLLIDER_SIZE.y * 0.75f, range, hitboxWidth};
  } else if (playerCurrentAnimation == "attack-side" && !playerFlipX) { // Right
    return Rectangle{playerPosition.x + Player::COLLIDER_SIZE.x / 2, playerPosition.y - Player::COLLIDER_SIZE.y * 0.75f, range, hitboxWidth};
  } else if (playerCurrentAnimation == "attack-down") { // Down
    return Rectangle{playerPosition.x - hitboxWidth / 2, playerPosition.y - Player::COLLIDER_SIZE.y * 0.25f, hitboxWidth, range};
  } else if (playerCurrentAnimation == "attack-up") { // Up
    return Rectangle{playerPosition.x - hitboxWidth / 2, playerPosition.y - Player::COLLIDER_SIZE.y - range, hitboxWidth, range};
  }
  return std::nullopt;
}

Animator Melee::buildAnimator(Assets& assets, Type type) {
  Spritesheet* sheet;
  switch (type) {
    case Type::HANDS: sheet = &assets.playerHandsSheet; break;
    case Type::BAT: sheet = &assets.batSheet; break;
  }

  Vector2 origin = Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y / 2 + Player::RENDER_SIZE.y / 2}; // match player's origin

  Animator anim = Animator(sheet, RENDER_SIZE, origin);

  anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  anim.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 10, true});
  anim.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 10, true});
  anim.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
  anim.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
  anim.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
  // anim.addAnimation("pickup-side", Animator::Animation{"pickup-side", {36, 37, 38}, 10, false});
  // anim.addAnimation("pickup-down", Animator::Animation{"pickup-down", {42, 43, 44}, 10, false});
  // anim.addAnimation("pickup-up", Animator::Animation{"pickup-up", {48, 49, 50}, 10, false});
  anim.addAnimation("attack-side", Animator::Animation{"attack-side", {54, 55, 56}, 10, false});
  anim.addAnimation("attack-down", Animator::Animation{"attack-down", {60, 61, 62}, 10, false});
  anim.addAnimation("attack-up", Animator::Animation{"attack-up", {66, 67, 68}, 10, false});
  anim.addAnimation("die", Animator::Animation{"die", {72, 73, 74, 75, 76, 77}, 10, false});

  return anim;
}

Melee::Properties Melee::getProperties(Type type) {
  switch (type) {
    case Type::HANDS: return HANDS_PROPERTIES;
    case Type::BAT: return BAT_PROPERTIES;
  }
}