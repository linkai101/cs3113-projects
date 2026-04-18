#pragma once
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  // ENVIRONMENT
  Texture2D terrainTexture;
  Spritesheet terrainSheet;

  // ENTITIES
  Texture2D playerTexture;
  Spritesheet playerSheet;

  Texture2D zombieTexture;
  Spritesheet zombieSheet;

  // MELEE
  Texture2D playerHandsTexture;
  Spritesheet playerHandsSheet;

  Texture2D batTexture;
  Spritesheet batSheet;
  Texture2D batPickableTexture;

  // GUNS
  Texture2D rifleTexture;
  Texture2D riflePickableTexture;
  Spritesheet rifleSheet;
  Texture2D rifleBulletTexture;

  Texture2D pistolTexture;
  Texture2D pistolPickableTexture;
  Spritesheet pistolSheet;
  Texture2D pistolBulletTexture;

  Texture2D shotgunTexture;
  Texture2D shotgunPickableTexture;
  Spritesheet shotgunSheet;
  Texture2D shotgunBulletTexture;

  // OTHER ITEMS
  Texture2D ammoCrateTexture;
  Spritesheet ammoCrateSheet;

  // UI
  Texture2D hotbarCellTexture;
  Texture2D hotbarCellSelectedTexture;

  Texture2D rifleBulletIconTexture;
  Texture2D pistolBulletIconTexture;
  Texture2D shotgunBulletIconTexture;

  void load() {
    // ENVIRONMENT
    terrainTexture = LoadTexture("assets/textures/tiles/terrain_yellow.png");
    terrainSheet = Spritesheet{terrainTexture, Vector2{16, 16}, 24};

    // ENTITIES
    playerTexture = LoadTexture("assets/textures/entities/player.png");
    playerSheet = Spritesheet{playerTexture, Vector2{19, 18}, 6};

    zombieTexture = LoadTexture("assets/textures/entities/zombie.png");
    zombieSheet = Spritesheet{zombieTexture, Vector2{19, 18}, 6};

    // MELEE
    playerHandsTexture = LoadTexture("assets/textures/gear/player_hands.png");
    playerHandsSheet = Spritesheet{playerHandsTexture, Vector2{29, 40}, 6};

    batTexture = LoadTexture("assets/textures/gear/bat.png");
    batSheet = Spritesheet{batTexture, Vector2{29, 40}, 6};
    batPickableTexture = LoadTexture("assets/textures/objects/pickable/bat.png");

    // GUNS
    rifleTexture = LoadTexture("assets/textures/guns/rifle.png");
    riflePickableTexture = LoadTexture("assets/textures/objects/pickable/rifle.png");
    rifleSheet = Spritesheet{rifleTexture, Vector2{20, 16}, 11};
    rifleBulletTexture = LoadTexture("assets/textures/guns/rifle_bullet.png");

    pistolTexture = LoadTexture("assets/textures/guns/pistol.png");
    pistolPickableTexture = LoadTexture("assets/textures/objects/pickable/pistol.png");
    pistolSheet = Spritesheet{pistolTexture, Vector2{20, 16}, 11};
    pistolBulletTexture = LoadTexture("assets/textures/guns/pistol_bullet.png");

    shotgunTexture = LoadTexture("assets/textures/guns/shotgun.png");
    shotgunPickableTexture = LoadTexture("assets/textures/objects/pickable/shotgun.png");
    shotgunSheet = Spritesheet{shotgunTexture, Vector2{20, 16}, 11};
    shotgunBulletTexture = LoadTexture("assets/textures/guns/shotgun_bullet.png");

    // OTHER ITEMS
    ammoCrateTexture = LoadTexture("assets/textures/objects/pickable/ammo_crate.png");
    ammoCrateSheet = Spritesheet{ammoCrateTexture, Vector2{9, 7}, 3};

    // UI
    hotbarCellTexture = LoadTexture("assets/textures/ui/hotbar_cell.png");
    hotbarCellSelectedTexture = LoadTexture("assets/textures/ui/hotbar_cell_selected.png");

    rifleBulletIconTexture = LoadTexture("assets/textures/ui/rifle_bullet.png");
    pistolBulletIconTexture = LoadTexture("assets/textures/ui/pistol_bullet.png");
    shotgunBulletIconTexture = LoadTexture("assets/textures/ui/shotgun_bullet.png");
  }

  void unload() {
    // ENVIRONMENT
    UnloadTexture(terrainTexture);

    // ENTITIES
    UnloadTexture(playerTexture);
    UnloadTexture(zombieTexture);

    // MELEE
    UnloadTexture(playerHandsTexture);
    UnloadTexture(batTexture);
    UnloadTexture(batPickableTexture);

    // GUNS
    UnloadTexture(rifleTexture);
    UnloadTexture(riflePickableTexture);
    UnloadTexture(rifleBulletTexture);

    UnloadTexture(pistolTexture);
    UnloadTexture(pistolPickableTexture);
    UnloadTexture(pistolBulletTexture);

    UnloadTexture(shotgunTexture);
    UnloadTexture(shotgunPickableTexture);
    UnloadTexture(shotgunBulletTexture);

    // OTHER ITEMS
    UnloadTexture(ammoCrateTexture);

    // UI
    UnloadTexture(hotbarCellTexture);
    UnloadTexture(hotbarCellSelectedTexture);

    UnloadTexture(rifleBulletIconTexture);
    UnloadTexture(pistolBulletIconTexture);
    UnloadTexture(shotgunBulletIconTexture);
  }
};
