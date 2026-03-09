#include "entity.h"

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

void Entity::update(float deltaTime) {
  if (animator.has_value()) animator->update(deltaTime);
}

void Entity::render() const {
  if (hasAnimator) {
    if (animator.has_value()) animator->render(position);
  } else {
    if (sprite.has_value()) sprite->render(position);
  }
}