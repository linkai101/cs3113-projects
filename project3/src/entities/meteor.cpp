#include "meteor.h"

Meteor::Meteor(
  Vector2 spawnPosition, 
  Animator animator
) :
  Entity(spawnPosition, animator)
{}

void Meteor::update(float deltaTime) {
  if (physicsBody.has_value()) {
    PhysicsBody& pb = *physicsBody;

    pb.acceleration.y = GRAVITY_ACCELERATION;
  }

  Entity::update(deltaTime);
}

void Meteor::render() const {
  Entity::render();
}