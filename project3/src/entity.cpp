#include "entity.h"

Entity::Entity(
  Vector2 position,
  Vector2 size,
  Vector2 origin,
  const char* textureFilePath,
  std::optional<Rectangle> textureArea
) :
  textureFilePath(textureFilePath),
  textureArea(textureArea),
  position(position),
  size(size),
  origin(origin)
{
  texture = LoadTexture(textureFilePath);
}

Entity::Entity(
  Vector2 position,
  Vector2 size,
  Vector2 origin,
  const char* textureFilePath,
  Vector2 textureSpriteSize,
  int textureSpriteColumns,
  int textureSpriteIndex
) :
  Entity(
    position,
    size,
    origin,
    textureFilePath,
    getSpritesheetSpriteTextureArea(textureSpriteSize, textureSpriteColumns, textureSpriteIndex)
  )
{}

Entity::~Entity() {
  UnloadTexture(texture);
}

void Entity::update(float deltaTime) {}

void Entity::render() const {
  // Use custom texture area if provided, otherwise use the entire texture
  Rectangle textureAreaActual = textureArea.value_or(Rectangle{ 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) });
  if (flipX) textureAreaActual.width = -textureAreaActual.width; // Apply flipX to texture area

  Rectangle destinationArea = { position.x, position.y, size.x, size.y };

  Vector2 originActual = flipX ? Vector2{ size.x - origin.x, origin.y } : origin; // Apply flipX to origin

  DrawTexturePro(
    texture,
    textureAreaActual,
    destinationArea,
    originActual,
    rotation,
    tint
  );

  // Debug: origin
  DrawCircle(position.x, position.y, 2, RED);

  // Debug: rendered bounds
  DrawRectangleLines(position.x - origin.x, position.y - origin.y, size.x, size.y, RED);
}

Rectangle Entity::getSpritesheetSpriteTextureArea(Vector2 spriteSize, int spriteColumns, int spriteIndex) {
  return {
    (spriteIndex % spriteColumns) * spriteSize.x,
    (spriteIndex / spriteColumns) * spriteSize.y,
    spriteSize.x,
    spriteSize.y
  };
}