#pragma once
#include "raylib.h"
#include "entity.h"

class Player : public Entity {
public:
  enum MoveDirection { NONE, LEFT, RIGHT };

  Player(
    Vector2 position,
    const float minX,
    const float maxX
  );
  ~Player();

  // void init() override;

  void update(float deltaTime) override;

  // void render() override;

  void move(MoveDirection moveDirection);

private:
  const float minX;
  const float maxX;
  float velocityX;      // pixels per second, + is right
  float angularVelocity; // degrees per second, + is clockwise

  static constexpr float VELOCITY = 500.0f;
  static constexpr float MAX_ROTATION = 12.0f;
  static constexpr float ANGULAR_VEL = 90.0f;
  static constexpr float RETURN_SPRING = 90.0f;
  static constexpr float RETURN_DAMPING = 5.0f;
};