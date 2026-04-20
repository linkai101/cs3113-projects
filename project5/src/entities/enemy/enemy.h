#pragma once
#include "entities/entity.h"
#include "entities/player.h"

class Enemy : public Entity {
public:
  using Entity::Entity;

  virtual void takeDamage(float amount) = 0;
  
  virtual void setTarget(Player* p) = 0;
};
