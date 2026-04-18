#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  Texture2D terrainTexture;
  Spritesheet terrainSheet;

  Texture2D playerTexture;
  Spritesheet playerSheet;

  Texture2D playerHandsTexture;
  Texture2D batTexture;
  Spritesheet playerHandsSheet;
  Spritesheet batSheet;

  Texture2D rifleTexture;
  Texture2D pistolTexture;
  Texture2D shotgunTexture;
  Spritesheet rifleSheet;
  Spritesheet pistolSheet;
  Spritesheet shotgunSheet;

  Texture2D rifleBulletTexture;
  Texture2D pistolBulletTexture;
  Texture2D shotgunBulletTexture;


  void load() {
    terrainTexture = LoadTexture("assets/textures/tiles/terrain_yellow.png");
    terrainSheet = Spritesheet{terrainTexture, Vector2{16, 16}, 24};

    playerTexture = LoadTexture("assets/textures/entities/player.png");
    playerSheet = Spritesheet{playerTexture, Vector2{19, 18}, 6};

    playerHandsTexture = LoadTexture("assets/textures/gear/player_hands.png");
    batTexture = LoadTexture("assets/textures/gear/bat.png");
    playerHandsSheet = Spritesheet{playerHandsTexture, Vector2{29, 40}, 6};
    batSheet = Spritesheet{batTexture, Vector2{29, 40}, 6};

    rifleTexture = LoadTexture("assets/textures/guns/rifle.png");
    pistolTexture = LoadTexture("assets/textures/guns/pistol.png");
    shotgunTexture = LoadTexture("assets/textures/guns/shotgun.png");
    rifleSheet = Spritesheet{rifleTexture, Vector2{20, 16}, 11};
    pistolSheet = Spritesheet{pistolTexture, Vector2{20, 16}, 11};
    shotgunSheet = Spritesheet{shotgunTexture, Vector2{20, 16}, 11};

    rifleBulletTexture = LoadTexture("assets/textures/guns/rifle_bullet.png");
    pistolBulletTexture = LoadTexture("assets/textures/guns/pistol_bullet.png");
    shotgunBulletTexture = LoadTexture("assets/textures/guns/shotgun_bullet.png");
  }

  void unload() {
    UnloadTexture(terrainTexture);

    UnloadTexture(playerTexture);

    UnloadTexture(playerHandsTexture);
    UnloadTexture(batTexture);

    UnloadTexture(rifleTexture);
    UnloadTexture(pistolTexture);
    UnloadTexture(shotgunTexture);
    
    UnloadTexture(rifleBulletTexture);
    UnloadTexture(pistolBulletTexture);
    UnloadTexture(shotgunBulletTexture);
  }
};
