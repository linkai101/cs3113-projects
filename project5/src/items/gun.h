#pragma once
#include "items/equippable.h"

class Gun : public Equippable {
public:
  enum class Type { RIFLE, PISTOL, SHOTGUN };

  struct Properties {
    float cooldown;
    float bulletSpeed;
    float maxRange;
    float damage;
    float bulletCount;
    float bulletSpread;
    int magazineSize;
    bool shellByShell;
  };

  Gun(Type type, Assets& assets);

  void update(float deltaTime) override;

  void render(Vector2 position) const override;

  void updateAngle(Vector2 playerWorldPos, Vector2 mouseWorldPos);

  void triggerShoot();

  void triggerReload(int amount);

  int cancelReload();

  Type getType() const { return type; }

  Properties getProperties() const { return properties; }

  bool canShoot() const { return cooldownTimer <= 0.0f && currentMag > 0 && (!reloading || properties.shellByShell); }
  
  bool isReloading() const { return reloading; }
  
  int getCurrentMag() const { return currentMag; }
  
  int getMagazineSize() const { return properties.magazineSize; }

private:
  Type type;
  Properties properties;

  int currentMag;
  int pendingReloadAmount = 0;
  bool reloading = false;

  float angle = 0.0f;
  float cooldownTimer = 0.0f;

  Animator animator;

  static Animator buildAnimator(Assets& assets, Type type);

  static Properties getProperties(Type type);

  static constexpr Vector2 RENDER_SIZE = {100, 80};
  static constexpr Vector2 RENDER_POSITION_OFFSET = {0, -36};

  static constexpr Properties RIFLE_PROPERTIES   = {0.15f, 700.0f, 900.0f, 25.0f, 1, 0.0f, 30, false};
  static constexpr Properties PISTOL_PROPERTIES  = {0.25f, 450.0f, 500.0f, 15.0f, 1, 0.0f, 12, false};
  static constexpr Properties SHOTGUN_PROPERTIES = {0.6f,  350.0f, 300.0f, 10.0f, 3, 0.2f,  6, true};
};