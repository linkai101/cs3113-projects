#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  Texture2D terrainTexture;
  Texture2D playerTexture;

  Spritesheet terrainSheet;
  Spritesheet playerSheet;

  void load() {
    terrainTexture = LoadTexture("assets/textures/tiles/terrain_yellow.png");
    playerTexture = LoadTexture("assets/textures/entities/player.png");

    terrainSheet = Spritesheet{terrainTexture, Vector2{16, 16}, 24};
    playerSheet = Spritesheet{playerTexture, Vector2{19, 18}, 6};
  }

  void unload() {
    UnloadTexture(terrainTexture);
    UnloadTexture(playerTexture);
  }
};
