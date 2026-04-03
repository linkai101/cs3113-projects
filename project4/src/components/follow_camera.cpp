#include "follow_camera.h"
#include <cmath>

FollowCamera::FollowCamera(int screenWidth, int screenHeight) :
  screenWidth(screenWidth), screenHeight(screenHeight)
{}

void FollowCamera::init(Vector2 target) {
  camera.target = target;
  camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

void FollowCamera::update(float deltaTime, Vector2 target) {
  float t = 1.0f - powf(1.0f - LERP_SPEED * deltaTime, 1.0f);
  camera.target.x = lerp(camera.target.x, target.x, t);
  camera.target.y = lerp(camera.target.y, target.y, t);
}

float FollowCamera::lerp(float a, float b, float t) {
  return a + t * (b - a);
}