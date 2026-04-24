#include "components/follow_camera.h"
#include <algorithm>
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

  float halfW = screenWidth / 2.0f;
  float halfH = screenHeight / 2.0f;

  if (mapWidth > 0 && mapWidth <= screenWidth) {
    camera.target.x = mapWidth / 2.0f;
  } else if (mapWidth > screenWidth) {
    camera.target.x = std::clamp(camera.target.x, halfW, mapWidth - halfW);
  }

  if (mapHeight > 0 && mapHeight <= screenHeight) {
    camera.target.y = mapHeight / 2.0f;
  } else if (mapHeight > screenHeight) {
    camera.target.y = std::clamp(camera.target.y, halfH, mapHeight - halfH);
  }
}

void FollowCamera::setBounds(float mapWidth, float mapHeight) {
  this->mapWidth = mapWidth;
  this->mapHeight = mapHeight;
}

float FollowCamera::lerp(float a, float b, float t) {
  return a + t * (b - a);
}