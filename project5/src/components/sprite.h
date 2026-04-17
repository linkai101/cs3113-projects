#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

class Sprite {
public:
  Sprite(
    Texture2D texture,
    Rectangle textureArea,
    Vector2 size,
    Vector2 origin,
    bool flipX = false
  );

  Sprite(
    Spritesheet* sheet,
    int frameIndex,
    Vector2 size,
    Vector2 origin,
    bool flipX = false
  );

  void render(Vector2 position) const;
  bool getFlipX() const { return flipX; }
  void setRotation(float degrees) { rotation = degrees; }

private:
  Texture2D texture;
  Rectangle textureArea; // region of the texture to render

  Vector2 size; // destination size to render
  Vector2 origin; // pivot point for rotation/flipping
  bool flipX;
  float rotation = 0.0f;
};