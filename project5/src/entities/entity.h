#pragma once
#include "raylib.h"
#include <string>
#include "components/sprite.h"
#include "components/animator.h"
#include "components/physics_body.h"
#include <optional>

class Entity {
public:
  enum class Direction { UP, DOWN, LEFT, RIGHT };

  Entity(Vector2 position);

  Entity(Vector2 position, Sprite sprite);

  Entity(Vector2 position, Animator animator);

  virtual ~Entity() = default;

  virtual void update(float deltaTime);

  virtual void resolveCollisions(std::vector<Entity*> entities);
  void resolveCollisions(const std::vector<Rectangle>& boxes);

  virtual void render() const;

  void enablePhysics(Vector2 colliderSize, Vector2 colliderOffset = {0, 0}, bool isStatic = false);

  void playAnimation(const std::string& name);

  const Vector2& getPosition() const { return position; }

  std::optional<Rectangle> getCollider() const {
    if (!physicsBody.has_value()) return std::nullopt;
    return physicsBody->getCollider(position);
  }

protected:
  Vector2 position;
  std::optional<PhysicsBody> physicsBody = std::nullopt;

  std::optional<Sprite> sprite;
  std::optional<Animator> animator;
  bool hasAnimator; // whether to use sprite or animator for rendering
};