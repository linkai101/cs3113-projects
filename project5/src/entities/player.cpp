#include "entities/player.h"

Player::Player(Vector2 spawnPosition, Assets& assets) :
  Entity(spawnPosition, buildAnimator(assets))
{
  enablePhysics(COLLIDER_SIZE, Vector2{-COLLIDER_SIZE.x / 2, -COLLIDER_SIZE.y}, false);
  playAnimation("idle-side");

  // Create gear layer animator
  gearLayer = Animator{
    &assets.playerHandsSheet,
    SIZE,
    Vector2{SIZE.x / 2, SIZE.y}
  };
  gearLayer->addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  gearLayer->addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 10, true});
  gearLayer->addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 10, true});
  gearLayer->addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
  gearLayer->addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
  gearLayer->addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
  gearLayer->addAnimation("pickup-side", Animator::Animation{"pickup-side", {36, 37, 38}, 10, false});
  gearLayer->addAnimation("pickup-down", Animator::Animation{"pickup-down", {42, 43, 44}, 10, false});
  gearLayer->addAnimation("pickup-up", Animator::Animation{"pickup-up", {48, 49, 50}, 10, false});
  gearLayer->addAnimation("punch-side", Animator::Animation{"punch-side", {54, 55, 56}, 10, false});
  gearLayer->addAnimation("punch-down", Animator::Animation{"punch-down", {60, 61, 62}, 10, false});
  gearLayer->addAnimation("punch-up", Animator::Animation{"punch-up", {66, 67, 68}, 10, false});
  gearLayer->play("idle-side");
}

void Player::processInput(bool up, bool down, bool left, bool right) {
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

void Player::update(float deltaTime) {
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;

    pb.velocity.y = movingUp ? -MOVEMENT_SPEED : (movingDown ? MOVEMENT_SPEED : 0);
    pb.velocity.x = movingLeft ? -MOVEMENT_SPEED : (movingRight ? MOVEMENT_SPEED : 0);
  }

  // Update animation
  std::string currentAnimation = animator->getCurrentAnimation();
  bool moving = movingUp || movingDown || movingLeft || movingRight;
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

  Entity::update(deltaTime);

  // Relay animator state to gear layer
  if (
    gearLayer->getCurrentAnimation() != currentAnimation
    && currentAnimation != "die"
  ) {
    gearLayer->play(currentAnimation);
  }
  gearLayer->setFlipX(animator->getFlipX());
  // Update gear layer
  gearLayer->update(deltaTime);
}

void Player::render() const {
  Entity::render();

  if (gearLayer.has_value() && animator->getCurrentAnimation() != "die") gearLayer->render(position);
}

Animator Player::buildAnimator(Assets& assets) {
  Animator playerAnimator = Animator(
    &assets.playerSheet,
    SIZE,
    Vector2{SIZE.x / 2, SIZE.y}
  );
  playerAnimator.addAnimation("idle-side", Animator::Animation{"idle-side", {0, 1, 2, 3, 4, 5}, 10, true});
  playerAnimator.addAnimation("idle-down", Animator::Animation{"idle-down", {6, 7, 8, 9, 10, 11}, 10, true});
  playerAnimator.addAnimation("idle-up", Animator::Animation{"idle-up", {12, 13, 14, 15, 16, 17}, 10, true});
  playerAnimator.addAnimation("run-side", Animator::Animation{"run-side", {18, 19, 20, 21, 22, 23}, 10, true});
  playerAnimator.addAnimation("run-down", Animator::Animation{"run-down", {24, 25, 26, 27, 28, 29}, 10, true});
  playerAnimator.addAnimation("run-up", Animator::Animation{"run-up", {30, 31, 32, 33, 34, 35}, 10, true});
  playerAnimator.addAnimation("pickup-side", Animator::Animation{"pickup-side", {36, 37, 38}, 10, false});
  playerAnimator.addAnimation("pickup-down", Animator::Animation{"pickup-down", {42, 43, 44}, 10, false});
  playerAnimator.addAnimation("pickup-up", Animator::Animation{"pickup-up", {48, 49, 50}, 10, false});
  playerAnimator.addAnimation("punch-side", Animator::Animation{"punch-side", {54, 55, 56}, 10, false});
  playerAnimator.addAnimation("punch-down", Animator::Animation{"punch-down", {60, 61, 62}, 10, false});
  playerAnimator.addAnimation("punch-up", Animator::Animation{"punch-up", {66, 67, 68}, 10, false});
  playerAnimator.addAnimation("die", Animator::Animation{"die", {72, 73, 74, 75, 76, 77}, 10, false});
  return playerAnimator;
}