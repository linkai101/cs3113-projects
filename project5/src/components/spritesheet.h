#pragma once
#include "raylib.h"

struct Spritesheet {
  Texture2D texture;
  Vector2 frameSize;
  int columns;

  Rectangle getFrame(int index) const;
};