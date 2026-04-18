#pragma once
#include "items/equippable/equippable.h"

class Melee : public Equippable {
public:
  enum class Type { HANDS, BAT };

  struct Properties {
    float cooldown;
    float hitboxWidth;
    float range;
    float damage;
  };

  Melee(Type type, Assets& assets);

  void update(float deltaTime) override;

  void render(Vector2 position) const override;

  void triggerStrike();

  std::optional<Rectangle> getHitRect() const;

  void setPlayerState(
    Vector2 position,
    std::string currentAnimation,
    int currentFrame,
    bool flipX
  ) {
    playerPosition = position;
    playerCurrentAnimation = currentAnimation;
    playerCurrentFrame = currentFrame;
    playerFlipX = flipX;
  }

  Type getType() const { return type; }

  Properties getProperties() const { return properties; }

  float getDamage() const { return properties.damage; }

  bool canStrike() const { return cooldownTimer <= 0.0f; }

private:
  Type type;
  Properties properties;

  Vector2 playerPosition;
  std::string playerCurrentAnimation;
  int playerCurrentFrame;
  bool playerFlipX;

  float cooldownTimer = 0.0f;

  Animator animator;

  static Animator buildAnimator(Assets& assets, Type type);

  static Properties getProperties(Type type);

  static constexpr Vector2 RENDER_SIZE = {145, 200};
  static constexpr Vector2 PLAYER_RENDER_SIZE = {95, 90};
  static constexpr Vector2 PLAYER_COLLIDER_SIZE = {40, 60};
  
  static constexpr Properties HANDS_PROPERTIES = {0.25f, 80.0f, 60.0f, 10.0f};
  static constexpr Properties BAT_PROPERTIES = {0.5f, 80.0f, 100.0f, 20.0f};
};