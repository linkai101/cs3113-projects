#include "entities/bullet.h"
#include "components/sprite.h"
#include "assets.h"
#include <cmath>

Bullet::Bullet(Vector2 position, float angleRad, Gun::Type type, Gun::Properties properties, Assets& assets) :
  Entity(position, Sprite(
    getBulletTexture(assets, type),
    Rectangle{0, 0, (float)getBulletTexture(assets, type).width, (float)getBulletTexture(assets, type).height},
    RENDER_SIZE,
    Vector2{RENDER_SIZE.x / 2, RENDER_SIZE.y / 2}
  )),
  velocity(calculateBulletVelocity(angleRad, properties.bulletSpeed)),
  maxRange(properties.maxRange),
  damage(properties.damage)
{
  sprite->setRotation(angleRad * RAD2DEG);
}

void Bullet::update(float deltaTime) {
  float dx = velocity.x * deltaTime;
  float dy = velocity.y * deltaTime;
  position.x += dx;
  position.y += dy;
  distanceTraveled += sqrtf(dx * dx + dy * dy);
}

Texture2D Bullet::getBulletTexture(Assets& assets, Gun::Type type) {
  switch (type) {
    case Gun::Type::RIFLE:
      return assets.rifleBulletTexture;
    case Gun::Type::SHOTGUN:
      return assets.shotgunBulletTexture;
    case Gun::Type::PISTOL:
      return assets.pistolBulletTexture;
  }
}

Vector2 Bullet::calculateBulletVelocity(float angleRad, float bulletSpeed) {
  return {cosf(angleRad) * bulletSpeed, sinf(angleRad) * bulletSpeed};
}