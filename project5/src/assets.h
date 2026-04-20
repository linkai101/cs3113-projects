#pragma once
#include <unordered_map>
#include <string>
#include "raylib.h"
#include "components/spritesheet.h"

struct Assets {
  // TILES
  Texture2D terrainYellowTexture;
  Spritesheet terrainYellowSheet;
  Texture2D buildingsBeigeTexture;
  Spritesheet buildingsBeigeSheet;
  Texture2D ironFenceTexture;
  Spritesheet ironFenceSheet;

  // ENTITIES
  Texture2D playerTexture;
  Spritesheet playerSheet;

  Texture2D zombieTexture;
  Spritesheet zombieSheet;
  
  Texture2D giantTexture;
  Spritesheet giantSheet;

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

  Spritesheet* getTileSheet(const std::string& name) {
    if (name == "terrain_yellow") return &terrainYellowSheet;
    if (name == "buildings_beige") return &buildingsBeigeSheet;
    if (name == "iron_fence") return &ironFenceSheet;
    return nullptr;
  }

  void load() {
    // TILES
    terrainYellowTexture = LoadTexture("assets/textures/tiles/terrain_yellow.png");
    terrainYellowSheet = Spritesheet{terrainYellowTexture, Vector2{16, 16}, 24};
    buildingsBeigeTexture = LoadTexture("assets/textures/tiles/buildings_beige.png");
    buildingsBeigeSheet = Spritesheet{buildingsBeigeTexture, Vector2{16, 16}, 13};
    ironFenceTexture = LoadTexture("assets/textures/tiles/iron_fence.png");
    ironFenceSheet = Spritesheet{ironFenceTexture, Vector2{16, 16}, 3};

    // ENTITIES
    playerTexture = LoadTexture("assets/textures/entities/player.png");
    playerSheet = Spritesheet{playerTexture, Vector2{19, 18}, 6};

    zombieTexture = LoadTexture("assets/textures/entities/zombie.png");
    zombieSheet = Spritesheet{zombieTexture, Vector2{19, 18}, 6};

    giantTexture = LoadTexture("assets/textures/entities/giant.png");
    giantSheet = Spritesheet{giantTexture, Vector2{30, 30}, 15};

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
    // TILES
    UnloadTexture(terrainYellowTexture);
    UnloadTexture(buildingsBeigeTexture);
    UnloadTexture(ironFenceTexture);

    // ENTITIES
    UnloadTexture(playerTexture);
    UnloadTexture(zombieTexture);
    UnloadTexture(giantTexture);

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
