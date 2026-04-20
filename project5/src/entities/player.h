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
  enum class EquipSlot { NONE, BAT, RIFLE, PISTOL, SHOTGUN };

  Player(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void move(bool up, bool down, bool left, bool right);

  void equip(EquipSlot slot);

  void attack();

  void reload();

  void takeDamage(float amount);

  void heal(float amount);

  bool canAttack() const;

  bool isDead() const { return state == State::DEAD; }

  float getHealth() const { return health; }

  float getDamageFlashIntensity() const { return damageFlashTimer / DAMAGE_FLASH_DURATION; }

  int getAmmoInventory(Gun::Type type) const;

  void addAmmo(Gun::Type type, int amount);

  float getAimAngle() const { return atan2f(mouseWorldPos.y - position.y, mouseWorldPos.x - position.x); }

  Vector2 getMouseWorldPosition() const { return mouseWorldPos; }

  Equippable* getEquipped() const { return equipped ? equipped : const_cast<Melee*>(&hands); }

  void setMouseWorldPosition(Vector2 pos) { mouseWorldPos = pos; }

  static constexpr Vector2 RENDER_SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {40, 60};
  static constexpr float MAX_HEALTH = 100.0f;

private:
  enum class State { ALIVE, DYING, DEAD };

  Vector2 mouseWorldPos = {0, 0};

  State state = State::ALIVE;
  float health = MAX_HEALTH;
  float damageFlashTimer = 0.0f;

  Direction facingDirection = Direction::RIGHT;
  bool movingUp = false;
  bool movingDown = false;
  bool movingLeft = false;
  bool movingRight = false;
  
  // Items
  // TODO: implement item inventory
  Melee hands; // Active when equipped is nullptr
  Melee bat;
  Gun rifle;
  Gun pistol;
  Gun shotgun;

  Equippable* equipped = nullptr;
  std::map<Gun::Type, int> ammoInventory;

  static Animator buildAnimator(Spritesheet* sheet);

  static constexpr float MOVEMENT_SPEED = 250.0f;
  static constexpr float DAMAGE_FLASH_DURATION = 0.12f;
};