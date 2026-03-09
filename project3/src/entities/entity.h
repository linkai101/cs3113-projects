#pragma once
#include "raylib.h"
#include "components/sprite.h"
#include "components/animator.h"
#include <optional>

/**
 * Entity.
 */
class Entity {
public:
  Entity(Vector2 position, Sprite sprite);

  Entity(Vector2 position, Animator animator);

  virtual ~Entity() = default;

  virtual void update(float deltaTime);

  virtual void render() const;

protected:
  Vector2 position;

  std::optional<Sprite> sprite;
  std::optional<Animator> animator;
  bool hasAnimator; // whether to use sprite or animator for rendering
};