#pragma once
#include "entities/entity.h"
#include "entities/bullet.h"
#include "assets.h"
#include "components/animator.h"

class Player : public Entity {
public:
  Player(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void move(bool up, bool down, bool left, bool right);

  void attack();

  void reload();

  void setMouseWorldPosition(Vector2 pos) { mouseWorldPos = pos; }

  bool canAttack() const;

  // Returns the bullet type if equipping a weapon, otherwise returns std::nullopt
  std::optional<BulletType> getEquippedBulletType() const;

  float getAimAngle() const;
  Vector2 getMouseWorldPosition() const { return mouseWorldPos; }

  void debug(int debugAction);

private:
  enum class Direction { UP, DOWN, LEFT, RIGHT };
  enum class AnimatorType { PLAYER, GEAR, WEAPON_RIFLE, WEAPON_PISTOL, WEAPON_SHOTGUN };
  enum class Equippable { HANDS, BAT, RIFLE, PISTOL, SHOTGUN };
  
  Vector2 mouseWorldPos = {0, 0};

  Direction facingDirection = Direction::RIGHT;
  bool movingUp = false;
  bool movingDown = false;
  bool movingLeft = false;
  bool movingRight = false;
  
  Equippable equipped = Equippable::HANDS;

  // Armor animators
  // Animator helmetAnimator; // TODO
  // Gear animators
  Animator handsAnimator;
  Animator batAnimator;
  // Weapon animators
  Animator rifleAnimator;
  Animator pistolAnimator;
  Animator shotgunAnimator;

  // Overlayed animator for equipped armor
  // Animator* armorLayer = nullptr; // TODO
  // Overlayed animator for equipped gear
  Animator* gearLayer = nullptr;
  // Overlayed animator for equipped weapon
  Animator* weaponLayer = nullptr;

  Animator buildAnimator(Spritesheet* sheet, AnimatorType type = AnimatorType::PLAYER);

  static constexpr Vector2 SIZE = {95, 90};
  static constexpr Vector2 GEAR_SIZE = {145, 200};
  static constexpr Vector2 WEAPON_SIZE = {100, 80};
  static constexpr Vector2 WEAPON_POSITION_OFFSET = {0, -36};
  static constexpr Vector2 COLLIDER_SIZE = {48, 80};
  static constexpr float MOVEMENT_SPEED = 250;
};