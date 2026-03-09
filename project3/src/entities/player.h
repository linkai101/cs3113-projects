#pragma once
#include "raylib.h"
#include "entities/entity.h"
#include "components/animator.h"

class Player : public Entity {
public:
  Player(Vector2 spawnPosition, Animator animator);

  void update(float deltaTime) override;

  void render() const override;

  // temp method for playing animations
  void playAnimation(const std::string& animationName);
};