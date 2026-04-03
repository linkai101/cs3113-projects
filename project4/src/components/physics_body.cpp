#include "components/physics_body.h"

Rectangle PhysicsBody::getCollider(Vector2 position) const {
  return {
    position.x + colliderOffset.x,
    position.y + colliderOffset.y,
    colliderSize.x,
    colliderSize.y
  };
}

void PhysicsBody::update(float deltaTime) {
  if (!isStatic) {
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
  }
}