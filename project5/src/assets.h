#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  Texture2D terrainTexture;
  Texture2D playerTexture;
  Texture2D playerHandsTexture;
  Texture2D batTexture;
  Texture2D rifleTexture;
  Texture2D pistolTexture;
  Texture2D shotgunTexture;

  Spritesheet terrainSheet;
  Spritesheet playerSheet;
  Spritesheet playerHandsSheet;
  Spritesheet batSheet;
  Spritesheet rifleSheet;
  Spritesheet pistolSheet;
  Spritesheet shotgunSheet;

  void load() {
    terrainTexture = LoadTexture("assets/textures/tiles/terrain_yellow.png");
    playerTexture = LoadTexture("assets/textures/entities/player.png");
    playerHandsTexture = LoadTexture("assets/textures/gear/player_hands.png");
    batTexture = LoadTexture("assets/textures/gear/bat.png");
    rifleTexture = LoadTexture("assets/textures/weapons/rifle.png");
    pistolTexture = LoadTexture("assets/textures/weapons/pistol.png");
    shotgunTexture = LoadTexture("assets/textures/weapons/shotgun.png");

    terrainSheet = Spritesheet{terrainTexture, Vector2{16, 16}, 24};
    playerSheet = Spritesheet{playerTexture, Vector2{19, 18}, 6};
    playerHandsSheet = Spritesheet{playerHandsTexture, Vector2{29, 40}, 6};
    batSheet = Spritesheet{batTexture, Vector2{29, 40}, 6};
    rifleSheet = Spritesheet{rifleTexture, Vector2{20, 16}, 11};
    pistolSheet = Spritesheet{pistolTexture, Vector2{20, 16}, 11};
    shotgunSheet = Spritesheet{shotgunTexture, Vector2{20, 16}, 11};
  }

  void unload() {
    UnloadTexture(terrainTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(playerHandsTexture);
    UnloadTexture(batTexture);
    UnloadTexture(rifleTexture);
    UnloadTexture(pistolTexture);
    UnloadTexture(shotgunTexture);
  }
};
