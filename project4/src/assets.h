#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  Texture2D islandBgTexture;
  Texture2D islandTerrainTexture;
  Texture2D smallMapTexture;
  Texture2D captainTexture;
  Texture2D crabbyTexture;
  Texture2D toothTexture;
  Texture2D starTexture;

  Spritesheet islandTerrainSheet;
  Spritesheet smallMapSheet;
  Spritesheet captainSheet;
  Spritesheet crabbySheet;
  Spritesheet toothSheet;
  Spritesheet starSheet;

  Music wavesMusic;
  Sound jumpSound;
  Sound hurtSound;
  Sound tapSound;

  void load() {
    islandBgTexture = LoadTexture("assets/textures/island-bg.png");

    islandTerrainTexture = LoadTexture("assets/textures/island-terrain.png");
    islandTerrainSheet = Spritesheet{islandTerrainTexture, Vector2{32, 32}, 17};

    smallMapTexture = LoadTexture("assets/textures/small-map.png");
    smallMapSheet = Spritesheet{smallMapTexture, Vector2{20, 20}, 8};

    captainTexture = LoadTexture("assets/textures/captain.png");
    captainSheet = Spritesheet{captainTexture, Vector2{64, 40}, 6};

    crabbyTexture = LoadTexture("assets/textures/crabby.png");
    crabbySheet = Spritesheet{crabbyTexture, Vector2{72, 32}, 9};

    toothTexture = LoadTexture("assets/textures/tooth.png");
    toothSheet = Spritesheet{toothTexture, Vector2{34, 30}, 8};

    starTexture = LoadTexture("assets/textures/star.png");
    starSheet = Spritesheet{starTexture, Vector2{34, 30}, 8};

    wavesMusic = LoadMusicStream("assets/textures/waves-loop.wav");
    jumpSound = LoadSound("assets/textures/jump.wav");
    hurtSound = LoadSound("assets/textures/hurt.wav");
    tapSound = LoadSound("assets/textures/tap.wav");
  }

  void unload() {
    UnloadTexture(islandBgTexture);
    UnloadTexture(islandTerrainTexture);
    UnloadTexture(smallMapTexture);
    UnloadTexture(captainTexture);
    UnloadTexture(crabbyTexture);
    UnloadTexture(toothTexture);
    UnloadTexture(starTexture);
    
    UnloadMusicStream(wavesMusic);
    UnloadSound(jumpSound);
    UnloadSound(hurtSound);
    UnloadSound(tapSound);
  }
};
