#include "entities/bullet.h"
#include "components/sprite.h"
#include "assets.h"
#include <cmath>

Bullet::Bullet(Vector2 position, float angleRad, BulletType type, Assets& assets) :
  Entity(position, Sprite(
    getBulletTexture(assets, type),
    Rectangle{0, 0, (float)getBulletTexture(assets, type).width, (float)getBulletTexture(assets, type).height},
    RENDER_SIZE,
    Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y / 2}
  ))
{
  velocity = {cosf(angleRad) * getBulletSpeed(type), sinf(angleRad) * getBulletSpeed(type)};
  maxRange = getBulletMaxRange(type);
  sprite->setRotation(angleRad * RAD2DEG);
}

void Bullet::update(float deltaTime) {
  float dx = velocity.x * deltaTime;
  float dy = velocity.y * deltaTime;
  position.x += dx;
  position.y += dy;
  distanceTraveled += sqrtf(dx * dx + dy * dy);
}

bool Bullet::isExpired() const {
  return distanceTraveled >= maxRange;
}

Texture2D Bullet::getBulletTexture(Assets& assets, BulletType type) {
  switch (type) {
    case BulletType::RIFLE:
      return assets.rifleBulletTexture;
    case BulletType::SHOTGUN:
      return assets.shotgunBulletTexture;
    case BulletType::PISTOL:
      return assets.pistolBulletTexture;
  }
}

float Bullet::getBulletSpeed(BulletType type) {
  switch (type) {
    case BulletType::RIFLE:
      return RIFLE_SPEED;
    case BulletType::SHOTGUN:
      return SHOTGUN_SPEED;
    case BulletType::PISTOL:
      return PISTOL_SPEED;
  }
}

float Bullet::getBulletMaxRange(BulletType type) {
  switch (type) {
    case BulletType::RIFLE:
      return RIFLE_MAX_RANGE;
    case BulletType::SHOTGUN:
      return SHOTGUN_MAX_RANGE;
    case BulletType::PISTOL:
      return PISTOL_MAX_RANGE;
  }
}