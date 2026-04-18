#pragma once
#include <map>
#include "entities/entity.h"
#include "items/equippable/equippable.h"
#include "items/equippable/melee.h"
#include "items/equippable/gun.h"
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

  float getAimAngle() const { return atan2f(mouseWorldPos.y - position.y, mouseWorldPos.x - position.x); }

  Vector2 getMouseWorldPosition() const { return mouseWorldPos; }

  Equippable* getEquipped() const { return equipped ? equipped : const_cast<Melee*>(&hands); }

  int getAmmoInventory(Gun::Type type) const;

  void debug(int debugAction);

private:
  Vector2 mouseWorldPos = {0, 0};

  Direction facingDirection = Direction::RIGHT;
  bool movingUp = false;
  bool movingDown = false;
  bool movingLeft = false;
  bool movingRight = false;
  
  Melee hands;

  // Items
  // TODO: implement item inventory
  Melee bat;
  Gun rifle;
  Gun pistol;
  Gun shotgun;

  Equippable* equipped = nullptr;

  std::map<Gun::Type, int> ammoInventory;

  static Animator buildAnimator(Spritesheet* sheet);

  static constexpr Vector2 SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {40, 60};
  static constexpr float MOVEMENT_SPEED = 250.0f;
};