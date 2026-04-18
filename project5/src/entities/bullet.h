#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "items/equippable/gun.h"
#include "assets.h"

class Bullet : public Entity {
public:
  Bullet(Vector2 position, float angleRad, Gun::Type type, Gun::Properties properties, Assets& assets);

  void update(float deltaTime) override;
  
  float getDamage() const { return damage; }

  bool isExpired() const { return distanceTraveled >= maxRange; }

private:
  Vector2 velocity;
  float maxRange;
  float damage;
  float distanceTraveled = 0.0f;

  static Texture2D getBulletTexture(Assets& assets, Gun::Type type);

  static Vector2 calculateBulletVelocity(float angleRad, float bulletSpeed);

  static constexpr Vector2 RENDER_SIZE = {20.0f, 8.0f};
};
