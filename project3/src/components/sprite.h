#pragma once
#include "raylib.h"
#include "spritesheet.h"

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
  
private:
  Texture2D texture;
  Rectangle textureArea; // region of the texture to render

  Vector2 size; // destination size to render
  Vector2 origin; // pivot point for rotation/flipping
  bool flipX;
};