#pragma once
#include "entities/entity.h"
#include "components/sprite.h"
#include "assets.h"
#include <optional>

class Item {
public:
  Item() = default;

  Item(Spritesheet* sheet, int frameIndex, Vector2 groundRenderSize);

  Item(Texture2D texture, Vector2 groundRenderSize);

  virtual ~Item() = default;

  bool isGrounded() const { return groundEntity != nullptr; }

  Entity* getGroundEntity() const { return groundEntity.get(); }

  std::optional<Rectangle> getCollider() const {
    return groundEntity ? groundEntity->getCollider() : std::nullopt;
  }

protected:
  void initGroundSprite(Texture2D texture, Vector2 groundRenderSize);

  void placeOnGround(Vector2 position);

private:
  std::optional<Sprite> groundSprite;
  Vector2 groundRenderSize = {0, 0};

  std::unique_ptr<Entity> groundEntity;
};
