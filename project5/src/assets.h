#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  Texture2D terrainTexture;
  Texture2D playerTexture;
  Texture2D playerHandsTexture;
  Texture2D batTexture;

  Spritesheet terrainSheet;
  Spritesheet playerSheet;
  Spritesheet playerHandsSheet;
  Spritesheet batSheet;

  void load() {
    terrainTexture = LoadTexture("assets/textures/tiles/terrain_yellow.png");
    playerTexture = LoadTexture("assets/textures/entities/player.png");
    playerHandsTexture = LoadTexture("assets/textures/entities/player_hands.png");
    batTexture = LoadTexture("assets/textures/entities/bat.png");

    terrainSheet = Spritesheet{terrainTexture, Vector2{16, 16}, 24};
    playerSheet = Spritesheet{playerTexture, Vector2{19, 18}, 6};
    playerHandsSheet = Spritesheet{playerHandsTexture, Vector2{29, 40}, 6};
    batSheet = Spritesheet{batTexture, Vector2{29, 40}, 6};
  }

  void unload() {
    UnloadTexture(terrainTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(playerHandsTexture);
    UnloadTexture(batTexture);
  }
};
