#include "scenes/level2.h"
#include "utils/color.h"

Level2::Level2(
  int screenWidth, int screenHeight,
  Spritesheet& islandTerrainSheet, Spritesheet& smallMapSheet, Texture2D& islandBgTexture
) :
  Scene(
    screenWidth, screenHeight, getTilePosition(MAP_SPAWN_POSITION), 
    islandTerrainSheet, smallMapSheet
  ),
  islandBgTexture(islandBgTexture)
{}

void Level2::loadLevel() {
  backgroundEntities.push_back(new Entity(
    {0, 0},
    Sprite(
      islandBgTexture,
      Rectangle{0, 0, static_cast<float>(islandBgTexture.width), static_cast<float>(islandBgTexture.height)},
      Vector2{MAP_COLS * TILE_SIZE, MAP_ROWS * TILE_SIZE},
      Vector2{0, 0}
    )
  ));
  
  loadTileGrid(&TERRAIN_MAP[0][0], MAP_ROWS, MAP_COLS, islandTerrainSheet, {0, 0}, true, false);
}
