#include "player.h"

const std::map<int, std::vector<int>> Player::ANIMATION_ATLAS = {
  {STANDING_IDLE, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }},
  {STANDING_WALK, { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }},
  {STANDING_RUN, { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }},
  {STANDING_JUMP, { 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 }},
  {STANDING_DIE, { 55, 56, 57, 58, 59 }},
  {FLYING, { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 }},
  {FLYING_NO_MOVEMENT, { 75, 76, 77, 78, 79, 80, 81, 82, 83, 84 }},
  {FLYING_DIE, { 85, 86, 87, 88, 89 }}
};

Player::Player(
  Vector2 position
) :
  AnimatedEntity(
    position,
    { 180.0f, 156.0f }, // size
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/jetpackman.png", // textureFilePath
    { 692.0f, 599.0f }, // textureFrameSize
    5, // textureFrameColumns
    ANIMATION_ATLAS, // animationAtlas
    20, // animationFPS
    STANDING_IDLE // idleAnimation
  )
{}

Player::~Player() {}

void Player::update(float deltaTime) {
  AnimatedEntity::update(deltaTime);
}

void Player::render() {
  AnimatedEntity::render();
}