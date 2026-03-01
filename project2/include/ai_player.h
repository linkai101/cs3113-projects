#pragma once
#include "player.h"
#include "ball.h"

class AIPlayer : public Player {
public:
  AIPlayer(
    Vector2 position,
    const float minX,
    const float maxX,
    float sideMaxX,   // right edge of AI's side (screen center x)
    Ball** balls,
    int ballCount
  );
  ~AIPlayer();

  void update(float deltaTime) override;

private:
  float sideMaxX;
  Ball** balls;
  int ballCount;

  static constexpr float DEAD_ZONE = 10.0f; // pixels, stop moving when close enough
};
