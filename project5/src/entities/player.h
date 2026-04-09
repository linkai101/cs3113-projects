#pragma once
#include "entities/entity.h"
#include "assets.h"
#include "components/animator.h"

class Player : public Entity {
public:
  Player(Vector2 spawnPosition, Assets& assets);

  void processInput(bool up, bool down, bool left, bool right);

  void update(float deltaTime) override;

  void render() const override;

private:
  enum class Direction { UP, DOWN, LEFT, RIGHT };
  
  Direction facingDirection = Direction::RIGHT;
  bool movingUp = false;
  bool movingDown = false;
  bool movingLeft = false;
  bool movingRight = false;

  Animator handsAnimator;
  // Overlayed animator for hands or the equipped item
  Animator* gearLayer;

  Animator buildAnimator(Spritesheet* sheet);

  static constexpr Vector2 SIZE = {95, 90};
  static constexpr Vector2 COLLIDER_SIZE = {48, 80};
  static constexpr float MOVEMENT_SPEED = 250;
};