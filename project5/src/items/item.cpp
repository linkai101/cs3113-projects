#include "items/item.h"

Item::Item(Spritesheet* sheet, int frameIndex, Vector2 groundRenderSize) :
  groundSprite(Sprite(sheet, frameIndex, groundRenderSize, Vector2{groundRenderSize.x / 2, groundRenderSize.y})),
  groundRenderSize(groundRenderSize)
{}

Item::Item(Texture2D texture, Vector2 groundRenderSize) :
  groundSprite(
    Sprite(
      texture,
      Rectangle{0, 0, (float)texture.width, (float)texture.height}, // texture area
      groundRenderSize,
      Vector2{groundRenderSize.x / 2, groundRenderSize.y} // origin
    )),
  groundRenderSize(groundRenderSize)
{}

void Item::initGroundSprite(Texture2D texture, Vector2 size) {
  groundRenderSize = size;
  groundSprite = Sprite(
    texture,
    Rectangle{0, 0, (float)texture.width, (float)texture.height},
    size,
    Vector2{size.x / 2, size.y}
  );
}

void Item::placeOnGround(Vector2 position) {
  groundEntity = std::make_unique<Entity>(position, *groundSprite);
  groundEntity->enablePhysics(groundRenderSize, Vector2{-groundRenderSize.x / 2, -groundRenderSize.y}, true);
}
