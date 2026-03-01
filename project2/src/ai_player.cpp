#include "ai_player.h"

AIPlayer::AIPlayer(
  Vector2 position,
  const float minX,
  const float maxX,
  float sideMaxX,
  Ball** balls,
  int ballCount
) :
  Player(position, minX, maxX),
  sideMaxX(sideMaxX),
  balls(balls),
  ballCount(ballCount)
{}

AIPlayer::~AIPlayer() {}

void AIPlayer::update(float deltaTime) {
  // Find closest ball
  Ball* target = nullptr;
  float lowestY = -1.0f;

  for (int i = 0; i < ballCount; i++) {
    if (balls[i] == nullptr) continue;
    Rectangle rect = balls[i]->getRect();
    float centerX = rect.x + rect.width / 2.0f;
    float bottomY = rect.y + rect.height;

    if (centerX <= sideMaxX && bottomY > lowestY) {
      lowestY = bottomY;
      target = balls[i];
    }
  }

  // If no ball on our side, fall back to whichever ball is lowest overall
  if (target == nullptr) {
    for (int i = 0; i < ballCount; i++) {
      if (balls[i] == nullptr) continue;
      Rectangle rect = balls[i]->getRect();
      float bottomY = rect.y + rect.height;
      if (bottomY > lowestY) {
        lowestY = bottomY;
        target = balls[i];
      }
    }
  }

  if (target != nullptr) {
    Rectangle rect = target->getRect();
    float ballCenterX = rect.x + rect.width / 2.0f;

    if (ballCenterX < position.x - DEAD_ZONE) {
      move(LEFT);
    } else if (ballCenterX > position.x + DEAD_ZONE) {
      move(RIGHT);
    } else {
      move(NONE);
    }
  } else {
    move(NONE);
  }

  Player::update(deltaTime);
}
