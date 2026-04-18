#include <algorithm>
#include "items/equippable/gun.h"

Gun::Gun(Type type, Assets& assets) :
  Equippable(getPickableTexture(assets, type), getPickableRenderSize(assets, type)),
  type(type),
  animator(buildAnimator(assets, type)),
  properties(getProperties(type)),
  currentMag(getProperties(type).magazineSize)
{}

void Gun::update(float deltaTime) {
  // Update reload state
  if (reloading && animator.isAnimationDone()) {
    if (properties.shellByShell) {
      currentMag = std::min(currentMag + 1, properties.magazineSize);
      pendingReloadAmount--;
      if (pendingReloadAmount > 0) {
        animator.play("reload");
      } else {
        reloading = false;
      }
    } else {
      currentMag = std::min(currentMag + pendingReloadAmount, properties.magazineSize);
      pendingReloadAmount = 0;
      reloading = false;
    }
  }
  if (!reloading && animator.isAnimationDone()) animator.play("idlerun");
  
  // Update animator
  if (fabsf(angle) <= 90.0f) {
    animator.setFlipX(false);
    animator.setRotation(angle);
  } else {
    animator.setFlipX(true);
    animator.setRotation(angle - 180.0f);
  }
  animator.update(deltaTime);

  // Update cooldown timer
  if (cooldownTimer > 0.0f) cooldownTimer -= deltaTime;
}

void Gun::render(Vector2 position) const {
  animator.render({position.x + RENDER_POSITION_OFFSET.x, position.y + RENDER_POSITION_OFFSET.y});
}

void Gun::updateAngle(Vector2 playerWorldPos, Vector2 mouseWorldPos) {
  float dx = mouseWorldPos.x - playerWorldPos.x - RENDER_POSITION_OFFSET.x;
  float dy = mouseWorldPos.y - playerWorldPos.y - RENDER_POSITION_OFFSET.y;
  angle = atan2f(dy, dx) * RAD2DEG;
}

void Gun::triggerShoot() {
  animator.play("shoot");
  cooldownTimer = properties.cooldown;
  if (currentMag > 0) currentMag--;
}

void Gun::triggerReload(int amount) {
  if (amount <= 0 || reloading) return;
  pendingReloadAmount = amount;
  reloading = true;
  animator.play("reload");
}

int Gun::cancelReload() {
  int refund = pendingReloadAmount;
  pendingReloadAmount = 0;
  reloading = false;
  return refund;
}

Animator Gun::buildAnimator(Assets& assets, Type type) {
  Spritesheet* sheet;
  switch (type) {
    case Type::RIFLE: sheet = &assets.rifleSheet; break;
    case Type::PISTOL: sheet = &assets.pistolSheet; break;
    case Type::SHOTGUN: sheet = &assets.shotgunSheet; break;
  }

  Vector2 origin = Vector2{24, RENDER_SIZE.y * 0.5f};

  Animator anim = Animator(sheet, RENDER_SIZE, origin);

  switch (type) {
    case Type::RIFLE:
      anim.addAnimation("idlerun", Animator::Animation{"idlerun", {0, 1, 2, 3, 4, 5}, 10, true});
      anim.addAnimation("reload", Animator::Animation{"reload", {11, 12, 13, 14, 15, 16, 17, 18}, 10, false});
      anim.addAnimation("shoot", Animator::Animation{"shoot", {22, 23, 24}, 10, false});
      break;
    case Type::PISTOL:
      anim.addAnimation("idlerun", Animator::Animation{"idlerun", {0, 1, 2, 3, 4, 5}, 10, true});
      anim.addAnimation("reload", Animator::Animation{"reload", {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}, 10, false});
      anim.addAnimation("shoot", Animator::Animation{"shoot", {22, 23, 24}, 10, false});
      break;
    case Type::SHOTGUN:
      anim.addAnimation("idlerun", Animator::Animation{"idlerun", {0, 1, 2, 3, 4, 5}, 10, true});
      anim.addAnimation("reload", Animator::Animation{"reload", {11, 12, 13, 14, 15, 16, 17, 18, 19}, 10, false});
      anim.addAnimation("shoot", Animator::Animation{"shoot", {22, 23, 24, 25, 26}, 10, false});
      break;
  };

  return anim;
}

Gun::Properties Gun::getProperties(Type type) {
  switch (type) {
    case Type::RIFLE: return RIFLE_PROPERTIES;
    case Type::PISTOL: return PISTOL_PROPERTIES;
    case Type::SHOTGUN: return SHOTGUN_PROPERTIES;
  }
}

Texture2D& Gun::getPickableTexture(Assets& assets, Type type) {
  switch (type) {
    case Type::RIFLE: return assets.riflePickableTexture;
    case Type::PISTOL: return assets.pistolPickableTexture;
    case Type::SHOTGUN: return assets.shotgunPickableTexture;
  }
}

Vector2 Gun::getPickableRenderSize(Assets& assets, Type type) {
  switch (type) {
    case Type::RIFLE: return RIFLE_PICKABLE_RENDER_SIZE;
    case Type::PISTOL: return PISTOL_PICKABLE_RENDER_SIZE;
    case Type::SHOTGUN: return SHOTGUN_PICKABLE_RENDER_SIZE;
  }
}