#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "assets.h"

enum class BulletType { PISTOL, RIFLE, SHOTGUN };

class Bullet : public Entity {
public:
  Bullet(Vector2 position, float angleRad, BulletType type, Assets& assets);

  void update(float deltaTime) override;
  
  float getDamage() const { return damage; }

  bool isExpired() const;

private:
  BulletType type;
  Vector2 velocity;
  float maxRange;
  float damage;
  float distanceTraveled = 0.0f;

  static Texture2D getBulletTexture(Assets& assets, BulletType type);

  static float getBulletSpeed(BulletType type);

  static float getBulletMaxRange(BulletType type);

  static float getBulletDamage(BulletType type);

  static constexpr Vector2 RENDER_SIZE = {20.0f, 8.0f};
  static constexpr float RIFLE_SPEED = 700;
  static constexpr float RIFLE_MAX_RANGE = 900;
  static constexpr float RIFLE_DAMAGE = 25.0f;
  static constexpr float SHOTGUN_SPEED = 350;
  static constexpr float SHOTGUN_MAX_RANGE = 300;
  static constexpr float SHOTGUN_DAMAGE = 10.0f;
  static constexpr float PISTOL_SPEED = 450;
  static constexpr float PISTOL_MAX_RANGE = 500;
  static constexpr float PISTOL_DAMAGE = 15.0f;
};
