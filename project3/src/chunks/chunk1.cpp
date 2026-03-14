#include "chunk1.h"

Chunk1::Chunk1(
  int screenWidth, int screenHeight,
  Spritesheet& tilesSheet, Spritesheet& grassSheet, Spritesheet& waterSheet, Spritesheet& meteorSheet,
  Texture2D& skyTexture, Texture2D& mountains1Texture, Texture2D& mountains2Texture
) :
  Chunk(screenWidth, screenHeight, tilesSheet, grassSheet, waterSheet, meteorSheet),
  skyTexture(skyTexture),
  mountains1Texture(mountains1Texture),
  mountains2Texture(mountains2Texture)
{}

void Chunk1::resolveCollisions(Player* player) {
  if (!loaded) return;
  if (!player->getPhysicsBody()) return;
  PhysicsBody& pb = *player->getPhysicsBody();

  // Invisible right wall collision
  Rectangle playerBox = pb.getCollider(player->getPosition());
  if (playerBox.x + playerBox.width > static_cast<float>(screenWidth)) {
    player->getPosition().x -= playerBox.x + playerBox.width - static_cast<float>(screenWidth);
    pb.velocity.x = 0;
  }

  // Check player falling off screen
  if (player->getPosition().y - pb.getCollider(player->getPosition()).height > static_cast<float>(screenHeight) + 500) {
    player->setGameState(Player::PlayerGameState::DEAD);
  }

  Chunk::resolveCollisions(player);
}

void Chunk1::render(Player* player) const {
  Chunk::render(player);

  DrawText(
    "Land here to win!",
    screenWidth - MeasureText("Land here to win!", 15) / 2 - 4.5f * TILE_SIZE,
    5 * TILE_SIZE - 30,
    15,
    WHITE
  );
}

void Chunk1::loadMap() {
  // Create background
  createBackgroundLayer(skyTexture);
  createBackgroundLayer(mountains1Texture);
  createBackgroundLayer(mountains2Texture);

  // Create tiles
  createTile({ 0, 2 }, TileType::BRIDGE_MIDDLE);
  createTile({ 1, 2 }, TileType::BRIDGE_RIGHT);
  createTile({ 2, 0 }, TileType::LEFT);
  createTile({ 2, 1 }, TileType::LEFT);
  createTile({ 2, 2 }, TileType::TOP_LEFT);
  createTile({ 3, 0 }, TileType::INNER);
  createTile({ 3, 1 }, TileType::INNER);
  createTile({ 3, 2 }, TileType::TOP);
  createTile({ 4, 0 }, TileType::RIGHT);
  createTile({ 4, 1 }, TileType::RIGHT);
  createTile({ 4, 2 }, TileType::TOP_RIGHT);

  // Create winning platform
  createTile({ 19, 8 }, TileType::BLOCK, true, true);
  createTile({ 20, 8 }, TileType::BLOCK, true, true);
  createTile({ 21, 8 }, TileType::BLOCK, true, true);

  // Create water
  createWater({ 0, 0 });
  createWater({ 0, 1 }, true);
  createWater({ 1, 0 });
  createWater({ 1, 1 }, true);
  for (int x = 5; x <= 25; x++) {
    createWater({ static_cast<float>(x), 0 });
    createWater({ static_cast<float>(x), 1 }, true);
  }

  // Create meteors
  Animator meteorAnimator = Animator(
    &meteorSheet,
    Vector2{ 64, 128 },
    Vector2{ 35, 96 }
  );
  meteorAnimator.addAnimation("meteor", Animator::Animation{"meteor", { 0, 1, 2, 3, 4 }, 10, true});
  meteorAnimator.play("meteor");

  for (int x = 13; x <= 15; x++) {
    Meteor meteor = Meteor(
      getPositionFromTileCoordinates({ static_cast<float>(x), 12.0f - x }, screenWidth, screenHeight),
      meteorAnimator
    );
    meteor.enablePhysics(Vector2{ 32, 32 }, Vector2{ -16, -16 }, false);
    meteorEntities.push_back(std::make_unique<Meteor>(meteor));
    allDeadly.push_back(meteorEntities.back().get());
  }
}
