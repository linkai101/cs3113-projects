#include "entities/player.h"

Player::Player(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(&assets.playerSheet)),
  handsAnimator(buildAnimator(&assets.playerHandsSheet, AnimatorType::GEAR)),
  batAnimator(buildAnimator(&assets.batSheet, AnimatorType::GEAR)),
  rifleAnimator(buildAnimator(&assets.rifleSheet, AnimatorType::WEAPON_RIFLE)),
  pistolAnimator(buildAnimator(&assets.pistolSheet, AnimatorType::WEAPON_PISTOL)),
  shotgunAnimator(buildAnimator(&assets.shotgunSheet, AnimatorType::WEAPON_SHOTGUN))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");
}

void Player::update(float deltaTime) {
  // Handle equipped
  gearLayer = nullptr;
  weaponLayer = nullptr;
  switch (equipped) {
    case Equippable::HANDS:
      gearLayer = &handsAnimator;
      break;
    case Equippable::BAT:
      gearLayer = &batAnimator;
      break;
    case Equippable::RIFLE:
      weaponLayer = &rifleAnimator;
      break;
    case Equippable::PISTOL:
      weaponLayer = &pistolAnimator;
      break;
    case Equippable::SHOTGUN:
      weaponLayer = &shotgunAnimator;
      break;
  }

  // Update player movement
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;

    pb.velocity.y = movingUp ? -MOVEMENT_SPEED : (movingDown ? MOVEMENT_SPEED : 0);
    pb.velocity.x = movingLeft ? -MOVEMENT_SPEED : (movingRight ? MOVEMENT_SPEED : 0);
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

  // Update gear layer
  if (gearLayer) {
    // Relay animator state to gear layer
    if (gearLayer->getCurrentAnimation() != currentAnimation) gearLayer->play(currentAnimation);
    gearLayer->setFlipX(animator->getFlipX());

    gearLayer->update(deltaTime);
  }

  // Update weapon layer
  if (weaponLayer) {
    if (weaponLayer->isAnimationDone()) {
      weaponLayer->play("idlerun");
    }

    float dx = mouseWorldPos.x - position.x;
    float dy = mouseWorldPos.y - position.y;
    float angle = atan2f(dy, dx) * RAD2DEG;

    if (fabsf(angle) <= 90.0f) {
      weaponLayer->setFlipX(false);
      weaponLayer->setRotation(angle);
    } else {
      weaponLayer->setFlipX(true);
      weaponLayer->setRotation(angle - 180.0f);
    }

    weaponLayer->update(deltaTime);
  }

  Entity::update(deltaTime);
}

void Player::render() const {
  bool weaponBehind = mouseWorldPos.y < position.y;

  if (weaponLayer && weaponBehind) weaponLayer->render({position.x, position.y - 20});

  Entity::render();

  if (gearLayer) gearLayer->render(position);
  if (weaponLayer && !weaponBehind) weaponLayer->render({position.x, position.y - 20});
}

void Player::move(bool up, bool down, bool left, bool right) {
  movingUp = up && !down;
  movingDown = down && !up;
  movingLeft = left && !right;
  movingRight = right && !left;

  if (movingLeft) {
    facingDirection = Direction::LEFT;
  } else if (movingRight) {
    facingDirection = Direction::RIGHT;
  } else if (movingUp) {
    facingDirection = Direction::UP;
  } else if (movingDown) {
    facingDirection = Direction::DOWN;
  }
}

Animator Player::buildAnimator(Spritesheet* sheet, AnimatorType type) {
  Vector2 size;
  Vector2 origin;

  // set size and origin based on type
  if (type == AnimatorType::PLAYER) {
    size = SIZE;
    origin = Vector2{SIZE.x / 2, SIZE.y};
  } else if (type == AnimatorType::GEAR) {
    size = GEAR_SIZE;
    origin = Vector2{GEAR_SIZE.x / 2, GEAR_SIZE.y / 2 + SIZE.y / 2}; // match player's origin
  } else if (
    type == AnimatorType::WEAPON_RIFLE ||
    type == AnimatorType::WEAPON_PISTOL ||
    type == AnimatorType::WEAPON_SHOTGUN
  ) {
    size = WEAPON_SIZE;
    origin = Vector2{24, WEAPON_SIZE.y - WEAPON_SIZE.y * 0.4f};
  }

  Animator anim = Animator(
    sheet,
    size,
    origin
  );

  // add animations based on type
  if (
    type == AnimatorType::PLAYER ||
    type == AnimatorType::GEAR
  ) {
    anim.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
    anim.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 10, true});
    anim.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 10, true});
    anim.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
    anim.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
    anim.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
    anim.addAnimation("pickup-side", Animator::Animation{"pickup-side", {36, 37, 38}, 10, false});
    anim.addAnimation("pickup-down", Animator::Animation{"pickup-down", {42, 43, 44}, 10, false});
    anim.addAnimation("pickup-up", Animator::Animation{"pickup-up", {48, 49, 50}, 10, false});
    anim.addAnimation("attack-side", Animator::Animation{"attack-side", {54, 55, 56}, 10, false});
    anim.addAnimation("attack-down", Animator::Animation{"attack-down", {60, 61, 62}, 10, false});
    anim.addAnimation("attack-up", Animator::Animation{"attack-up", {66, 67, 68}, 10, false});
    anim.addAnimation("die", Animator::Animation{"die", {72, 73, 74, 75, 76, 77}, 10, false});
  } else if (type == AnimatorType::WEAPON_RIFLE) {
    anim.addAnimation("idlerun", Animator::Animation{"idlerun", {0, 1, 2, 3, 4, 5}, 10, true});
    anim.addAnimation("reload", Animator::Animation{"reload", {11, 12, 13, 14, 15, 16, 17, 18}, 10, false});
    anim.addAnimation("shoot", Animator::Animation{"shoot", {22, 23, 24}, 10, false});
  } else if (type == AnimatorType::WEAPON_PISTOL) {
    anim.addAnimation("idlerun", Animator::Animation{"idlerun", {0, 1, 2, 3, 4, 5}, 10, true});
    anim.addAnimation("reload", Animator::Animation{"reload", {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}, 10, false});
    anim.addAnimation("shoot", Animator::Animation{"shoot", {22, 23, 24}, 10, false});
  } else if (type == AnimatorType::WEAPON_SHOTGUN) {
    anim.addAnimation("idlerun", Animator::Animation{"idlerun", {0, 1, 2, 3, 4, 5}, 10, true});
    anim.addAnimation("reload", Animator::Animation{"reload", {11, 12, 13, 14, 15, 16, 17, 18, 19}, 10, false});
    anim.addAnimation("shoot", Animator::Animation{"shoot", {22, 23, 24, 25, 26}, 10, false});
  }


  return anim;
}

void Player::debug(int debugAction) {
  switch (debugAction) {
    case 1: // equip hands
      equipped = Equippable::HANDS;
      break;
    case 2: // equip bat
      equipped = Equippable::BAT;
      break;
    case 3: // equip rifle
      equipped = Equippable::RIFLE;
      break;
    case 4: // equip pistol
      equipped = Equippable::PISTOL;
      break;
    case 5: // equip shotgun
      equipped = Equippable::SHOTGUN;
      break;
    case 0: // attack
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
      if (weaponLayer) weaponLayer->play("shoot");
      break;
    case 9: // reload
      if (weaponLayer) weaponLayer->play("reload");
      break;
    case 8: // pickup
      switch (facingDirection) {
        case Direction::UP:
          animator->play("pickup-up");
          break;
        case Direction::DOWN:
          animator->play("pickup-down");
          break;
        case Direction::LEFT:
          animator->play("pickup-side");
          animator->setFlipX(true);
          break;
        case Direction::RIGHT:
          animator->play("pickup-side");
          animator->setFlipX(false);
          break;
      }
      break;
    case 7: // die
      animator->play("die");
      break;
  }
}