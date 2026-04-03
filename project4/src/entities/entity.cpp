#include "entities/entity.h"

Entity::Entity(Vector2 position) :
  position(position),
  hasAnimator(false)
{}

Entity::Entity(Vector2 position, Sprite sprite) :
  position(position),
  sprite(sprite),
  hasAnimator(false)
{}

Entity::Entity(Vector2 position, Animator animator) :
  position(position),
  animator(animator),
  hasAnimator(true)
{}

void Entity::enablePhysics(Vector2 colliderSize, Vector2 colliderOffset, bool isStatic) {
  physicsBody = PhysicsBody{ colliderSize, colliderOffset, isStatic };
}

void Entity::playAnimation(const std::string& name) {
  if (hasAnimator && animator.has_value()) animator->play(name);
}

bool Entity::getFlipX() const {
  if (hasAnimator && animator.has_value()) return animator->getFlipX();
  if (sprite.has_value()) return sprite->getFlipX();
  return false;
}

void Entity::update(float deltaTime) {
  if (animator.has_value()) animator->update(deltaTime);

  // Handle physics logic
  if (physicsBody.has_value()) {
    physicsBody->update(deltaTime);

    // Update position if the body is not static
    if (!physicsBody->isStatic) {
      position.x += physicsBody->velocity.x * deltaTime;
      position.y += physicsBody->velocity.y * deltaTime;
    }
  }
}

void Entity::render() const {
  if (hasAnimator) {
    if (animator.has_value()) animator->render(position);
  } else {
    if (sprite.has_value()) sprite->render(position);
  }

  if (physicsBody.has_value()) {
    // DEBUG: collider bounds
    Rectangle collider = physicsBody->getCollider(position);
    DrawRectangleLines(
      collider.x,
      collider.y,
      collider.width,
      collider.height,
      BLUE
    );
  }
}

bool Entity::isColliding(Entity* a, Entity* b) {
  Rectangle aRect = a->physicsBody->getCollider(a->position);
  Rectangle bRect = b->physicsBody->getCollider(b->position);

  return (
    aRect.x < bRect.x + bRect.width &&
    aRect.x + aRect.width > bRect.x &&
    aRect.y < bRect.y + bRect.height &&
    aRect.y + aRect.height > bRect.y
  );
}