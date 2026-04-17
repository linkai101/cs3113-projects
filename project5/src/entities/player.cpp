#include "entities/player.h"

Player::Player(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(&assets.playerSheet)),
  handsAnimator(buildAnimator(&assets.playerHandsSheet, AnimatorType::GEAR)),
  batAnimator(buildAnimator(&assets.batSheet, AnimatorType::GEAR))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");
}

void Player::update(float deltaTime) {
  // Update equipped layer
  if (batEquipped) equippedLayer = &batAnimator;
  else equippedLayer = &handsAnimator;

  // Update movement
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;

    pb.velocity.y = movingUp ? -MOVEMENT_SPEED : (movingDown ? MOVEMENT_SPEED : 0);
    pb.velocity.x = movingLeft ? -MOVEMENT_SPEED : (movingRight ? MOVEMENT_SPEED : 0);
  }

  // Update animation
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

  Entity::update(deltaTime);

  // Relay animator state to equipped layer
  if (equippedLayer) {
    if (equippedLayer->getCurrentAnimation() != currentAnimation) equippedLayer->play(currentAnimation);
    equippedLayer->setFlipX(animator->getFlipX());
    // Update equipped layer
    equippedLayer->update(deltaTime);
  }
}

void Player::render() const {
  Entity::render();

  if (equippedLayer) equippedLayer->render(position);
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

  switch (type) {
    case AnimatorType::PLAYER:
      size = SIZE;
      origin = Vector2{SIZE.x / 2, SIZE.y};
      break;
    case AnimatorType::GEAR:
      size = GEAR_SIZE;
      origin = Vector2{GEAR_SIZE.x / 2, GEAR_SIZE.y / 2 + SIZE.y / 2}; // match player's origin
      break;
  }

  Animator a = Animator(
    sheet,
    size,
    origin
  );
  a.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  a.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 10, true});
  a.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 10, true});
  a.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
  a.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
  a.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
  a.addAnimation("pickup-side", Animator::Animation{"pickup-side", {36, 37, 38}, 10, false});
  a.addAnimation("pickup-down", Animator::Animation{"pickup-down", {42, 43, 44}, 10, false});
  a.addAnimation("pickup-up", Animator::Animation{"pickup-up", {48, 49, 50}, 10, false});
  a.addAnimation("attack-side", Animator::Animation{"attack-side", {54, 55, 56}, 10, false});
  a.addAnimation("attack-down", Animator::Animation{"attack-down", {60, 61, 62}, 10, false});
  a.addAnimation("attack-up", Animator::Animation{"attack-up", {66, 67, 68}, 10, false});
  a.addAnimation("die", Animator::Animation{"die", {72, 73, 74, 75, 76, 77}, 10, false});
  return a;
}

void Player::debug(int debugAction) {
  switch (debugAction) {
    case 0: // toggle bat
      batEquipped = !batEquipped;
      break;
    case 1: // pickup
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
    case 2: // attack
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
      break;
    case 3: // die
      animator->play("die");
      break;
  }
}