#include <algorithm>
#include "entities/player.h"
#include "sound_manager.h"

Player::Player(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(&assets.playerSheet)),
  hands(Melee::Type::HANDS, assets),
  bat(Melee::Type::BAT, assets),
  rifle(Gun::Type::RIFLE, assets),
  pistol(Gun::Type::PISTOL, assets),
  shotgun(Gun::Type::SHOTGUN, assets)
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");
}

void Player::update(float deltaTime) {
  // Handle dying state
  if (state == State::DYING) {
    if (damageFlashTimer > 0.0f) {
      damageFlashTimer -= deltaTime;
      if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
    }
    Entity::update(deltaTime);
    if (animator.has_value() && animator->isAnimationDone()) {
      state = State::DEAD;
      physicsBody = std::nullopt;
    }
    return;
  }

  // Handle dead state
  if (state == State::DEAD) return;

  // Update damage flash timer
  if (damageFlashTimer > 0.0f) {
    damageFlashTimer -= deltaTime;
    if (damageFlashTimer < 0.0f) damageFlashTimer = 0.0f;
  }

  // Update player movement
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;

    pb.velocity.y = movingUp ? -MOVEMENT_SPEED : (movingDown ? MOVEMENT_SPEED : 0);
    pb.velocity.x = movingLeft ? -MOVEMENT_SPEED : (movingRight ? MOVEMENT_SPEED : 0);
  }

  // Update facing direction from mouse position
  float mouseAngle = atan2f(mouseWorldPos.y - position.y, mouseWorldPos.x - position.x);
  if (mouseAngle > -PI / 4 && mouseAngle <= PI / 4) {
    facingDirection = Direction::RIGHT;
  } else if (mouseAngle > PI / 4 && mouseAngle <= 3 * PI / 4) {
    facingDirection = Direction::DOWN;
  } else if (mouseAngle > -3 * PI / 4 && mouseAngle <= -PI / 4) {
    facingDirection = Direction::UP;
  } else {
    facingDirection = Direction::LEFT;
  }

  // Update player animations
  std::string currentAnimation = animator->getCurrentAnimation();
  bool moving = movingUp || movingDown || movingLeft || movingRight;
  // Wait for active animation to finish
  if (animator->isAnimationLooping() || animator->isAnimationDone()) {
    if (moving) {
      switch (facingDirection) {
        case Direction::UP:
          if (currentAnimation != "run-up") playAnimation("run-up");
          break;
        case Direction::DOWN:
          if (currentAnimation != "run-down") playAnimation("run-down");
          break;
        case Direction::LEFT:
          if (currentAnimation != "run-side") playAnimation("run-side");
          animator->setFlipX(true);
          break;
        case Direction::RIGHT:
          if (currentAnimation != "run-side") playAnimation("run-side");
          animator->setFlipX(false);
          break;
      }
    } else {
      switch (facingDirection) {
        case Direction::UP:
          if (currentAnimation != "idle-up") playAnimation("idle-up");
          break;
        case Direction::DOWN:
          if (currentAnimation != "idle-down") playAnimation("idle-down");
          break;
        case Direction::LEFT:
          if (currentAnimation != "idle-side") playAnimation("idle-side");
          animator->setFlipX(true);
          break;
        case Direction::RIGHT:
          if (currentAnimation != "idle-side") playAnimation("idle-side");
          animator->setFlipX(false);
          break;
      }
    }
  }

  if (equipped) {
    if (auto gun = dynamic_cast<Gun*>(equipped)) {
      // Update angle for guns
      gun->updateAngle(position, mouseWorldPos);
    } else if (auto melee = dynamic_cast<Melee*>(equipped)) {
      // Update hands animator state
      melee->setPlayerState(position, currentAnimation, animator->getCurrentFrame(), animator->getFlipX());
    }

    // Update equipped item
    equipped->update(deltaTime);
  } else {
    // Update hands
    hands.setPlayerState(position, currentAnimation, animator->getCurrentFrame(), animator->getFlipX());
    hands.update(deltaTime);
  }

  Entity::update(deltaTime);
}

void Player::render() const {
  const bool alive = state == State::ALIVE;
  const Melee* melee = dynamic_cast<const Melee*>(equipped);
  const Gun* gun = dynamic_cast<const Gun*>(equipped);

  const bool gunBehindPlayer = facingDirection == Direction::UP;

  if (alive && gun && gunBehindPlayer) equipped->render(position);
  Entity::render();
  if (alive && ((gun && !gunBehindPlayer) || melee)) equipped->render(position);
  if (alive && !equipped) hands.render(position);

  // Render damage flash
  if (damageFlashTimer > 0.0f && hasAnimator && animator.has_value()) {
    float alpha = damageFlashTimer / DAMAGE_FLASH_DURATION;
    BeginBlendMode(BLEND_ADDITIVE);
    animator->render(position, Fade(WHITE, alpha));
    EndBlendMode();
  }
}

void Player::move(bool up, bool down, bool left, bool right) {
  if (state != State::ALIVE) return;
  movingUp = up && !down;
  movingDown = down && !up;
  movingLeft = left && !right;
  movingRight = right && !left;
}

void Player::equip(EquipSlot slot) {
  equipped = nullptr;
  switch (slot) {
    case EquipSlot::NONE: equipped = nullptr; break;
    case EquipSlot::BAT: equipped = &bat; break;
    case EquipSlot::RIFLE: equipped = &rifle; break;
    case EquipSlot::PISTOL: equipped = &pistol; break;
    case EquipSlot::SHOTGUN: equipped = &shotgun; break;
  }
}

void Player::takeDamage(float amount) {
  if (state != State::ALIVE) return;

  damageFlashTimer = DAMAGE_FLASH_DURATION;
  health -= amount;

  if (health <= 0.0f) {
    health = 0.0f;
    state = State::DYING;
    movingUp = movingDown = movingLeft = movingRight = false;
    if (physicsBody.has_value()) physicsBody->velocity = {0, 0};
    playAnimation("die");
  }
}

void Player::heal(float amount) {
  if (state != State::ALIVE) return;
  health = std::min(health + amount, MAX_HEALTH);
}

void Player::attack() {
  if (state != State::ALIVE) return;
  auto playMeleePlayerAnimation = [this]() -> void {
    switch (facingDirection) {
      case Direction::UP:
        animator->play("attack-up");
        break;
      case Direction::DOWN:
        animator->play("attack-down");
        break;
      case Direction::LEFT:
        animator->play("attack-side");
        animator->setFlipX(true);
        break;
      case Direction::RIGHT:
        animator->play("attack-side");
        animator->setFlipX(false);
        break;
    }
  };

  if (equipped) {
    if (auto gun = dynamic_cast<Gun*>(equipped)) {
      // Gun shoot
      // Cancel in-progress reload, refunding unloaded shells
      if (gun->isReloading()) ammoInventory[gun->getType()] += gun->cancelReload();
      gun->triggerShoot();
    } else if (auto melee = dynamic_cast<Melee*>(equipped)) {
      // Melee strike
      playMeleePlayerAnimation();
      melee->triggerStrike();
      SoundManager::get().play(SFX::BAT_SWING);
    }
  } else {
    // Hands strike
    playMeleePlayerAnimation();
    hands.triggerStrike();
    SoundManager::get().play(SFX::PUNCH);
  }
}

void Player::reload() {
  if (state != State::ALIVE) return;
  if (auto gun = dynamic_cast<Gun*>(equipped)) {
    if (!gun->isReloading() && gun->getCurrentMag() < gun->getMagazineSize()) {
      int needed = gun->getMagazineSize() - gun->getCurrentMag();
      int& inv = ammoInventory[gun->getType()];
      int toAdd = std::min(needed, inv);
      if (toAdd > 0) {
        inv -= toAdd;
        gun->triggerReload(toAdd);
        SoundManager::get().play(gun->getProperties().shellByShell ? SFX::RELOAD_SHELL : SFX::RELOAD_MAG);
      }
    }
  }
}

bool Player::canAttack() const {
  if (equipped) {
    if (auto gun = dynamic_cast<Gun*>(equipped)) {
      // Check gun can shoot
      return gun->canShoot();
    } else if (auto melee = dynamic_cast<Melee*>(equipped)) {
      // Check melee can strike
      return melee->canStrike();
    }

    return false;
  }

  // Check hands can strike
  return hands.canStrike();
}

int Player::getAmmoInventory(Gun::Type type) const {
  auto it = ammoInventory.find(type);
  return it != ammoInventory.end() ? it->second : 0;
}

void Player::addAmmo(Gun::Type type, int amount) {
  ammoInventory[type] += amount;
}

Animator Player::buildAnimator(Spritesheet* sheet) {
  Vector2 origin = Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y};

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