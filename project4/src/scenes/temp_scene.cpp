#include "temp_scene.h"
#include "utils/color.h"

TempScene::TempScene(
  int screenWidth, int screenHeight,
  Spritesheet& islandTerrainSheet
) :
  Scene(
    screenWidth, screenHeight, getTilePosition({2, 2}), 
    islandTerrainSheet
  )
{}

void TempScene::resolveCollisions(Player* player) {
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

void TempScene::render(Player* player) const {
  Scene::render(player);
}

void TempScene::loadLevel() {
  loadTileGrid(&MAP[0][0], MAP_ROWS, MAP_COLS, islandTerrainSheet, {0, 0}, true, false);
}
