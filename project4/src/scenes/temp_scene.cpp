#include "temp_scene.h"
#include "utils/color.h"

TempScene::TempScene(
  int screenWidth, int screenHeight,
  Spritesheet& tilesSheet
) :
  Scene(screenWidth, screenHeight, getPositionFromTileCoordinates({3, 4}, screenWidth, screenHeight), tilesSheet)
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

void TempScene::loadMap() {
  // Create tiles
  createTile({ 0, 0 }, TileType::RIGHT);
  createTile({ 0, 1 }, TileType::RIGHT);
  createTile({ 0, 2 }, TileType::CORNER_BOTTOM_RIGHT);
  createTile({ 0, 3 }, TileType::TOP);
  createTile({ 1, 1 }, TileType::SLOPE_TOP_LEFT, false);
  createTile({ 1, 2 }, TileType::BOTTOM);
  createTile({ 1, 3 }, TileType::TOP);
  createTile({ 2, 2 }, TileType::BOTTOM);
  createTile({ 2, 3 }, TileType::TOP_ALT);
  createTile({ 3, 2 }, TileType::BOTTOM);
  createTile({ 3, 3 }, TileType::TOP);
  createTile({ 4, 2 }, TileType::BOTTOM_RIGHT);
  createTile({ 4, 3 }, TileType::TOP_RIGHT);

  createTile({ 7, 2 }, TileType::BLOCK);
  createTile({ 8, 2 }, TileType::BLOCK);

  createTile({ 11, 1 }, TileType::SLOPE_TOP_RIGHT, false);
  createTile({ 11, 2 }, TileType::TOP_LEFT);
  createTile({ 12, 0 }, TileType::LEFT);
  createTile({ 12, 1 }, TileType::CORNER_BOTTOM_LEFT);
  createTile({ 12, 2 }, TileType::CORNER_TOP_LEFT);
  createTile({ 12, 3 }, TileType::LEFT);
  createTile({ 12, 4 }, TileType::LEFT);
  createTile({ 12, 5 }, TileType::TOP_LEFT);
  createTile({ 13, 0 }, TileType::INNER);
  createTile({ 13, 1 }, TileType::INNER);
  createTile({ 13, 2 }, TileType::INNER);
  createTile({ 13, 3 }, TileType::INNER);
  createTile({ 13, 4 }, TileType::INNER);
  createTile({ 13, 5 }, TileType::TOP);
  createTile({ 14, 0 }, TileType::INNER);
  createTile({ 14, 1 }, TileType::INNER);
  createTile({ 14, 2 }, TileType::INNER);
  createTile({ 14, 3 }, TileType::INNER);
  createTile({ 14, 4 }, TileType::INNER_FOSSIL);
  createTile({ 14, 5 }, TileType::TOP);
  createTile({ 15, 0 }, TileType::INNER);
  createTile({ 15, 1 }, TileType::INNER);
  createTile({ 15, 2 }, TileType::INNER);
  createTile({ 15, 3 }, TileType::INNER);
  createTile({ 15, 4 }, TileType::INNER);
  createTile({ 15, 5 }, TileType::TOP);
  createTile({ 16, 0 }, TileType::INNER);
  createTile({ 16, 1 }, TileType::INNER);
  createTile({ 16, 2 }, TileType::CORNER_TOP_RIGHT);
  createTile({ 16, 3 }, TileType::RIGHT);
  createTile({ 16, 4 }, TileType::RIGHT);
  createTile({ 16, 5 }, TileType::TOP_RIGHT);
  createTile({ 17, 0 }, TileType::INNER);
  createTile({ 17, 1 }, TileType::INNER_FOSSIL);
  createTile({ 17, 2 }, TileType::TOP);
  createTile({ 18, 0 }, TileType::INNER);
  createTile({ 18, 1 }, TileType::INNER);
  createTile({ 18, 2 }, TileType::TOP);
  createTile({ 19, 0 }, TileType::RIGHT);
  createTile({ 19, 1 }, TileType::RIGHT);
  createTile({ 19, 2 }, TileType::TOP_RIGHT);
  createTile({ 20, 2 }, TileType::BRIDGE_LEFT);
  createTile({ 21, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 22, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 23, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 23, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 24, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 25, 2 }, TileType::BRIDGE_RIGHT);
}
