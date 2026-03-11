#include "tutorial_chunk.h"

TutorialChunk::TutorialChunk(
  int screenWidth, int screenHeight,
  Spritesheet& tilesSheet, Spritesheet& grassSheet, Spritesheet& waterSheet,
  Texture2D& skyTexture, Texture2D& mountains1Texture, Texture2D& mountains2Texture
) :
  Chunk(screenWidth, screenHeight, tilesSheet, grassSheet, waterSheet),
  skyTexture(skyTexture),
  mountains1Texture(mountains1Texture),
  mountains2Texture(mountains2Texture)
{}

void TutorialChunk::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;
  PhysicsBody& pb = *player->getPhysicsBody();

  // Invisible left wall collision
  Rectangle playerBox = pb.getCollider(player->getPosition());
  if (playerBox.x < 0) {
    player->getPosition().x -= playerBox.x;
    pb.velocity.x = 0;
  }

  // Invisible top wall collision
  if (playerBox.y < 0) {
    player->getPosition().y -= playerBox.y;
    pb.velocity.y = 0;
  }

  // Respawn player if they fall off the screen
  if (player->getPosition().y - pb.getCollider(player->getPosition()).height > static_cast<float>(screenHeight) + 500) {
    player->getPosition() = getPositionFromTileCoordinates({ 2, 5 }, screenWidth, screenHeight);
    pb.velocity = { 0, 0 };
    pb.acceleration = { 0, 0 };
    player->setJumping(false);
    player->setBoosting(false);
    player->setCanBoost(false);
  }

  Chunk::resolveCollisions(player);
}

void TutorialChunk::render() const {
  Chunk::render();
  
  DrawText(
    "<- [A]        [D] ->",
    TILE_SIZE * 2.0f - MeasureText("<- [A]        [D] ->", 15) / 2.0f,
    screenHeight - TILE_SIZE * 7,
    15,
    WHITE
  );

  DrawText(
    "/\\",
    TILE_SIZE * 8.0f - MeasureText("/\\", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6 - 30,
    15,
    WHITE
  );
  DrawText(
    "|",
    TILE_SIZE * 8.0f - MeasureText("|", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6 - 25,
    15,
    WHITE
  );
  DrawText(
    "[W / Space]",
    TILE_SIZE * 8.0f - MeasureText("[W / Space]", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6,
    15,
    WHITE
  );

  DrawText(
    "/\\",
    TILE_SIZE * 11.5f - MeasureText("/\\", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6 - 30,
    15,
    WHITE
  );
  DrawText(
    "|",
    TILE_SIZE * 11.5f - MeasureText("|", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6 - 25,
    15,
    WHITE
  );
  DrawText(
    "/\\",
    TILE_SIZE * 11.5f - MeasureText("/\\", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6 - 5,
    15,
    WHITE
  );
  DrawText(
    "|",
    TILE_SIZE * 11.5f - MeasureText("|", 15) / 2.0f,
    screenHeight - TILE_SIZE * 6,
    15,
    WHITE
  );
}

void TutorialChunk::loadMap() {
  // Create background
  createBackgroundLayer(skyTexture);
  createBackgroundLayer(mountains1Texture);
  createBackgroundLayer(mountains2Texture);

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

  // Create grass
  createGrass({ 0, 4 }, GrassType::GRASS_ARCH);
  createGrass({ 2, 4 }, GrassType::GRASS_2);
  createGrass({ 3, 1 }, GrassType::GRASS_HANGING, true);
  createGrass({ 4, 1 }, GrassType::GRASS_HANGING_DOUBLE, true);
  createGrass({ 4, 4 }, GrassType::GRASS_1);
  createGrass({ 13, 6 }, GrassType::GRASS_ARCH);
  createGrass({ 15, 6 }, GrassType::GRASS_SPIRAL_LEFT);
  createGrass({ 16, 6 }, GrassType::GRASS_SPIRAL_RIGHT);
  createGrass({ 17, 3 }, GrassType::GRASS_BOTTOM_LEFT);
  createGrass({ 19, 3 }, GrassType::GRASS_DOUBLE);

  // Create water
  createWater({ 20, 0 });
  createWater({ 20, 1 }, true);
  createWater({ 21, 0 });
  createWater({ 21, 1 }, true);
  createWater({ 22, 0 });
  createWater({ 22, 1 }, true);
  createWater({ 23, 0 });
  createWater({ 23, 1 }, true);
  createWater({ 24, 0 });
  createWater({ 24, 1 }, true);
  createWater({ 25, 0 });
  createWater({ 25, 1 }, true);
}
