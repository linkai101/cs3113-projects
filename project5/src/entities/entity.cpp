#include "entities/entity.h"
#include "utils/collision.h"

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

void Entity::resolveCollisions(std::vector<Entity*> entities) {
  if (!physicsBody.has_value()) return;
  PhysicsBody& pb = *physicsBody;

  // Resolve horizontal collisions
  for (Entity* entity : entities) {
    if (!entity->physicsBody) continue;

    Rectangle box = pb.getCollider(position);
    Rectangle otherBox = entity->physicsBody->getCollider(entity->position);

    if (!CheckRectCollision(box, otherBox)) continue;

    float overlapX = std::min(box.x + box.width,  otherBox.x + otherBox.width) - std::max(box.x, otherBox.x);
    float overlapY = std::min(box.y + box.height, otherBox.y + otherBox.height) - std::max(box.y, otherBox.y);

    if (overlapX < overlapY) {
      if (box.x < otherBox.x) {
        position.x -= overlapX;
      } else {
        position.x += overlapX;
      }
    }
  }
  // Resolve vertical collisions
  for (Entity* entity : entities) {
    if (!entity->physicsBody) continue;

    Rectangle box = pb.getCollider(position);
    Rectangle otherBox = entity->physicsBody->getCollider(entity->position);

    if (!CheckRectCollision(box, otherBox)) continue;

    float overlapX = std::min(box.x + box.width,  otherBox.x + otherBox.width) - std::max(box.x, otherBox.x);
    float overlapY = std::min(box.y + box.height, otherBox.y + otherBox.height) - std::max(box.y, otherBox.y);

    if (overlapY <= overlapX) {
      if (box.y < otherBox.y) {
        position.y -= overlapY;
      } else {
        position.y += overlapY;
      }
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