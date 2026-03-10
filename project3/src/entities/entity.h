#pragma once
#include "raylib.h"
#include "components/sprite.h"
#include "components/animator.h"
#include "components/physics_body.h"
#include <optional>

/**
 * Entity.
 */
class Entity {
public:
  Entity(Vector2 position);

  Entity(Vector2 position, Sprite sprite);

  Entity(Vector2 position, Animator animator);

  virtual ~Entity() = default;

  void enablePhysics(Vector2 colliderSize, Vector2 colliderOffset = {0, 0}, bool isStatic = false);

  bool getFlipX() const;

  virtual void update(float deltaTime);

  virtual void render() const;

  friend class Level;

protected:
  Vector2 position;
  std::optional<PhysicsBody> physicsBody = std::nullopt;

  std::optional<Sprite> sprite;
  std::optional<Animator> animator;
  bool hasAnimator; // whether to use sprite or animator for rendering
};