#pragma once
#include "raylib.h"
#include "entity.h"

class Ball : public Entity {
public:
  Ball(Vector2 position);
  ~Ball();

  // void init() override;

  void update(float deltaTime) override;

  // void render() override;

  void reflect(Vector2 surfaceNormal, float upwardAmplify = 1.0f, float maxUpwardSpeed = 0.0f);

  void reset(Vector2 position);

private:
  float velocityX; // pixels per second, + is right
  float velocityY; // pixels per second, + is down

  static constexpr float GRAVITY = 700.0f;
};