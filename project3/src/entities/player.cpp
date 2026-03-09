#include "player.h"

Player::Player(
  Vector2 spawnPosition,
  Animator animator
) :
  Entity(
    spawnPosition,
    animator
  )
{}

void Player::update(float deltaTime) {
  Entity::update(deltaTime);
}

void Player::render() const {
  Entity::render();
}

void Player::playAnimation(const std::string& animationName) {
  animator->play(animationName);
}