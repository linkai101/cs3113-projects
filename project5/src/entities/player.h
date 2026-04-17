#pragma once
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Player : public Entity {
public:
  Player(Vector2 spawnPosition, Assets& assets);

  void update(float deltaTime) override;

  void render() const override;

  void move(bool up, bool down, bool left, bool right);

  void debug(int debugAction);

private:
  enum class Direction { UP, DOWN, LEFT, RIGHT };
  enum class AnimatorType { PLAYER, GEAR, /* WEAPON */ };
  
  Direction facingDirection = Direction::RIGHT;
  bool movingUp = false;
  bool movingDown = false;
  bool movingLeft = false;
  bool movingRight = false;
  
  bool batEquipped = false;

  Animator handsAnimator;
  Animator batAnimator;
  // Animator helmetAnimator; // TODO

  // Overlayed animator for armor, if equipped
  Animator* armorLayer = nullptr;
  // Overlayed animator for hands or the equipped item
  Animator* equippedLayer = nullptr;

  Animator buildAnimator(Spritesheet* sheet, AnimatorType type = AnimatorType::PLAYER);

  static constexpr Vector2 SIZE = {95, 90};
  static constexpr Vector2 GEAR_SIZE = {145, 200}; // hands, bat, helmet
  // static constexpr Vector2 WEAPON_SIZE; // TODO
  static constexpr Vector2 COLLIDER_SIZE = {48, 80};
  static constexpr float MOVEMENT_SPEED = 250;
};