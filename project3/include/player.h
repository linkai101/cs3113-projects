#pragma once
#include "raylib.h"
#include "animated_entity.h"

class Player : public AnimatedEntity {
public:
  enum AnimationType {
    STANDING_IDLE,
    STANDING_WALK,
    STANDING_RUN,
    STANDING_JUMP,
    STANDING_DIE,
    FLYING,
    FLYING_NO_MOVEMENT,
    FLYING_DIE
  };

  Player(
    Vector2 position
  );

  ~Player();

  void update(float deltaTime) override;

  void render() override;

private:
  static const std::map<int, std::vector<int>> ANIMATION_ATLAS;
};