#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  Texture2D islandBgTexture;
  Texture2D islandTerrainTexture;
  Texture2D smallMapTexture;
  Texture2D captainTexture;

  Spritesheet islandTerrainSheet;
  Spritesheet smallMapSheet;
  Spritesheet captainSheet;

  void load() {
    islandBgTexture = LoadTexture("assets/textures/island-bg.png");

    islandTerrainTexture = LoadTexture("assets/textures/island-terrain.png");
    islandTerrainSheet = Spritesheet{islandTerrainTexture, Vector2{32, 32}, 17};

    smallMapTexture = LoadTexture("assets/textures/small-map.png");
    smallMapSheet = Spritesheet{smallMapTexture, Vector2{20, 20}, 8};

    captainTexture = LoadTexture("assets/textures/captain.png");
    captainSheet = Spritesheet{captainTexture, Vector2{64, 40}, 6};
  }

  void unload() {
    UnloadTexture(islandBgTexture);
    UnloadTexture(islandTerrainTexture);
    UnloadTexture(smallMapTexture);
    UnloadTexture(captainTexture);
  }
};
