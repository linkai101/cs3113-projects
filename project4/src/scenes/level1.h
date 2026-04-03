#pragma once
#include "scenes/scene.h"

class Level1 : public Scene {
public:
  Level1(
    int screenWidth, int screenHeight,
    Spritesheet& islandTerrainSheet, Spritesheet& smallMapSheet, Texture2D& islandBgTexture
  );

protected:
  void loadLevel() override;

private:
  Texture2D& islandBgTexture;
  
  static constexpr int E = EMPTY_TILE;
  static constexpr int MAP_ROWS = 6;
  static constexpr int MAP_COLS = 9;
  static constexpr int TERRAIN_MAP[MAP_ROWS][MAP_COLS] = {
    { 18, 6, 35, 27, 35, 35, 35, 35, 7 },
    { 6, 36, E, 38, E, E, E, E, 17 },
    { 19, E, E, E, E, E, E, E, 17 },
    { 19, E, E, E, E, E, E, E, 17 },
    { 19, E, E, E, E, 0, 1, 1, 24 },
    { 23, 1, 1, 1, 1, 24, 18, 18, 18 },
  };
  static constexpr Vector2 MAP_SPAWN_POSITION = {1.5f, 4.5f};
};