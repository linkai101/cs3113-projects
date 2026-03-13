#include "chunk1.h"

Chunk1::Chunk1(
  int screenWidth, int screenHeight,
  Spritesheet& tilesSheet, Spritesheet& grassSheet, Spritesheet& waterSheet,
  Texture2D& skyTexture, Texture2D& mountains1Texture, Texture2D& mountains2Texture
) :
  Chunk(screenWidth, screenHeight, tilesSheet, grassSheet, waterSheet),
  skyTexture(skyTexture),
  mountains1Texture(mountains1Texture),
  mountains2Texture(mountains2Texture)
{}

void Chunk1::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;
  PhysicsBody& pb = *player->getPhysicsBody();

  // Check player falling off screen
  if (player->getPosition().y - pb.getCollider(player->getPosition()).height > static_cast<float>(screenHeight) + 500) {
    player->setGameState(Player::PlayerGameState::DEAD);
  }

  Chunk::resolveCollisions(player);
}

void Chunk1::loadMap() {
  // Create background
  createBackgroundLayer(skyTexture);
  createBackgroundLayer(mountains1Texture);
  createBackgroundLayer(mountains2Texture);

  // Create tiles
  createTile({ 0, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 1, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 2, 2 }, TileType::BRIDGE_RIGHT);
  createTile({ 3, 0 }, TileType::LEFT);
  createTile({ 3, 1 }, TileType::LEFT);
  createTile({ 3, 2 }, TileType::TOP_LEFT);
  createTile({ 4, 0 }, TileType::INNER);
  createTile({ 4, 1 }, TileType::INNER);
  createTile({ 4, 2 }, TileType::TOP);
  createTile({ 5, 0 }, TileType::RIGHT);
  createTile({ 5, 1 }, TileType::RIGHT);
  createTile({ 5, 2 }, TileType::TOP_RIGHT);

  // Create water
  createWater({ 0, 0 });
  createWater({ 0, 1 }, true);
  createWater({ 1, 0 });
  createWater({ 1, 1 }, true);
  createWater({ 2, 0 });
  createWater({ 2, 1 }, true);
  for (int x = 6; x <= 25; x++) {
    createWater({ static_cast<float>(x), 0 });
    createWater({ static_cast<float>(x), 1 }, true);
  }
}
