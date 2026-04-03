#pragma once
#include "raylib.h"

class FollowCamera {
public:
  FollowCamera(int screenWidth, int screenHeight);

  void init(Vector2 target);

  void update(float deltaTime, Vector2 target);

  Camera2D get() const { return camera; }

private:
  int screenWidth;
  int screenHeight;
  Camera2D camera = { 0 };

  static float lerp(float a, float b, float t);

  static constexpr float LERP_SPEED = 5.0f;
};