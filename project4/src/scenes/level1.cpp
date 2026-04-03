#include "level1.h"
#include "utils/color.h"

Level1::Level1(
  int screenWidth, int screenHeight,
  Spritesheet& islandTerrainSheet
) :
  Scene(
    screenWidth, screenHeight, getTilePosition(MAP_SPAWN_POSITION), 
    islandTerrainSheet
  )
{}

void Level1::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;
  PhysicsBody& pb = *player->getPhysicsBody();

  // Invisible left wall collision
  Rectangle playerBox = pb.getCollider(player->getPosition());
  if (playerBox.x < 0) {
    player->getPosition().x -= playerBox.x;
    pb.velocity.x = 0;
  }

  Scene::resolveCollisions(player);
}

void Level1::loadLevel() {
  loadTileGrid(&TERRAIN_MAP[0][0], MAP_ROWS, MAP_COLS, islandTerrainSheet, {0, 0}, true, false);
}
