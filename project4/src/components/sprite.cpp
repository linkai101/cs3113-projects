#include "sprite.h"

Sprite::Sprite(
  Texture2D texture,
  Rectangle textureArea,
  Vector2 size,
  Vector2 origin,
  bool flipX
) :
  texture(texture),
  textureArea(textureArea),
  size(size),
  origin(origin),
  flipX(flipX)
{}

Sprite::Sprite(
  Spritesheet* sheet,
  int frameIndex,
  Vector2 size,
  Vector2 origin,
  bool flipX
) :
  texture(sheet->texture),
  textureArea(sheet->getFrame(frameIndex)),
  size(size),
  origin(origin),
  flipX(flipX)
{}

void Sprite::render(Vector2 position) const {
  Rectangle sourceArea = textureArea;
  if (flipX) sourceArea.width = -sourceArea.width; // Apply flipX to source area

  Rectangle destinationArea = { position.x, position.y, size.x, size.y };

  Vector2 originActual = flipX ? Vector2{ size.x - origin.x, origin.y } : origin; // Apply flipX to origin
  
  DrawTexturePro(
    texture,
    sourceArea,
    destinationArea,
    originActual,
    0.0f, // rotation
    WHITE // tint
  );

  // DEBUG: rendered bounds
  // DrawRectangleLines(position.x - origin.x, position.y - origin.y, size.x, size.y, RED);

  // DEBUG: origin
  // DrawCircle(position.x, position.y, 2, RED);
}