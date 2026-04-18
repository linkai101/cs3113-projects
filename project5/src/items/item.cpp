#include "items/item.h"

Item::Item(Spritesheet* sheet, int frameIndex, Vector2 groundRenderSize) :
  groundSprite(Sprite(sheet, frameIndex, groundRenderSize, Vector2{groundRenderSize.x / 2, groundRenderSize.y})),
  groundRenderSize(groundRenderSize)
{}

Item::Item(Texture2D texture, Rectangle textureArea, Vector2 groundRenderSize) :
  groundSprite(Sprite(texture, textureArea, groundRenderSize, Vector2{groundRenderSize.x / 2, groundRenderSize.y})),
  groundRenderSize(groundRenderSize)
{}

void Item::placeOnGround(Vector2 position) {
  groundEntity = std::make_unique<Entity>(position, *groundSprite);
  groundEntity->enablePhysics(groundRenderSize, Vector2{-groundRenderSize.x / 2, -groundRenderSize.y}, true);
}
