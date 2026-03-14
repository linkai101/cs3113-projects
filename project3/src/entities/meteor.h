#pragma once
#include "raylib.h"
#include "entities/entity.h"

class Meteor : public Entity {
public:
  Meteor(Vector2 spawnPosition, Animator animator);

  void update(float deltaTime) override;

  void render() const override;

private:
  static constexpr float GRAVITY_ACCELERATION = 600.0f;
};

