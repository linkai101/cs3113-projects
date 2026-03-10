#include "core/level.h"
#include "components/sprite.h"
#include "components/spritesheet.h"
#include <algorithm>

Level::Level(
  int screenWidth,
  int screenHeight
) :
  screenWidth(screenWidth),
  screenHeight(screenHeight)
{
  // Load textures / spritesheets
  skyTexture = LoadTexture("assets/textures/sky.png");
  mountains1Texture = LoadTexture("assets/textures/mountains_1.png");
  mountains2Texture = LoadTexture("assets/textures/mountains_2.png");

  tilesTexture = LoadTexture("assets/textures/tiles.png");
  tilesSheet = Spritesheet{tilesTexture, Vector2{ 16, 16 }, 4};

  playerTexture = LoadTexture("assets/textures/jetpackman.png");
  playerSheet = Spritesheet{playerTexture, Vector2{ 692, 599 }, 5};

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
  createTile({ 15, 0 }, TileType::INNER);
  createTile({ 16, 1 }, TileType::INNER);
  createTile({ 16, 2 }, TileType::CORNER_TOP_RIGHT);
  createTile({ 16, 3 }, TileType::RIGHT);
  createTile({ 16, 4 }, TileType::RIGHT);
  createTile({ 16, 5 }, TileType::TOP_RIGHT);
  createTile({ 17, 0 }, TileType::INNER);
  createTile({ 17, 1 }, TileType::INNER);
  createTile({ 17, 2 }, TileType::TOP);
  createTile({ 18, 0 }, TileType::INNER);
  createTile({ 18, 1 }, TileType::INNER);
  createTile({ 18, 2 }, TileType::TOP);
  createTile({ 19, 0 }, TileType::INNER);
  createTile({ 19, 1 }, TileType::INNER);
  createTile({ 19, 2 }, TileType::TOP);
  createTile({ 20, 0 }, TileType::INNER);
  createTile({ 20, 1 }, TileType::INNER);
  createTile({ 20, 2 }, TileType::TOP);
  createTile({ 21, 0 }, TileType::INNER);
  createTile({ 21, 1 }, TileType::INNER);
  createTile({ 21, 2 }, TileType::TOP);
  createTile({ 21, 0 }, TileType::INNER);
  createTile({ 22, 1 }, TileType::INNER);
  createTile({ 22, 2 }, TileType::TOP);
  createTile({ 23, 0 }, TileType::INNER);
  createTile({ 23, 1 }, TileType::INNER);
  createTile({ 23, 2 }, TileType::TOP);
  createTile({ 24, 0 }, TileType::INNER);
  createTile({ 24, 1 }, TileType::INNER);
  createTile({ 24, 2 }, TileType::TOP);

  // Create player
  Animator playerAnimator = Animator(
    &playerSheet,
    Vector2{ 150, 130 }, // size
    Vector2{ 90, 115 } // origin
  );
  playerAnimator.addAnimation("standing_idle", Animator::Animation{"standing_idle", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 10, true});
  playerAnimator.addAnimation("standing_walk", Animator::Animation{"standing_walk", { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }, 20, true});
  playerAnimator.addAnimation("standing_run", Animator::Animation{"standing_run", { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }, 30, true});
  playerAnimator.addAnimation("standing_jump", Animator::Animation{"standing_jump", { 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 }, 15, false});
  playerAnimator.addAnimation("standing_die", Animator::Animation{"standing_die", { 55, 56, 57, 58, 59 }, 15, false});
  playerAnimator.addAnimation("flying", Animator::Animation{"flying", { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 }, 20, true});
  playerAnimator.addAnimation("flying_no_movement", Animator::Animation{"flying_no_movement", { 75, 76, 77, 78, 79, 80, 81, 82, 83, 84 }, 20, true});
  playerAnimator.addAnimation("flying_die", Animator::Animation{"flying_die", { 85, 86, 87, 88, 89 }, 15, false});

  player = std::make_unique<Player>(
    getPositionFromTileCoordinates({ 2, 5 }, screenWidth, screenHeight),
    playerAnimator
  );
  player->playAnimation("standing_idle");

  player->enablePhysics(
    Vector2{ 50, 110 }, // colliderSize
    Vector2{ -25, -110 }, // colliderOffset
    false
  );
}

Level::~Level() {}

void Level::processInput() {
  bool left = IsKeyDown(KEY_A);
  bool right = IsKeyDown(KEY_D);
  bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE);

  player->move(left, right, up);
}

void Level::update(float deltaTime) {
  // Apply gravity to player
  if (player && player->physicsBody) {
    player->physicsBody->acceleration.y = player->isBoosting() ? -600.0f : GRAVITY;
  }

  for (auto& entity : collidableEntities) {
    entity->update(deltaTime);
  }

  if (player) player->update(deltaTime);

  resolveCollisions();
}

void Level::render() const {
  for (auto& entity : staticEntities) {
    entity->render();
  }
  for (auto& entity : collidableEntities) {
    entity->render();
  }

  if (player) player->render();
}

Vector2 Level::getPositionFromTileCoordinates(Vector2 tileCoordinates, int screenWidth, int screenHeight) {
  return {
    static_cast<float>(tileCoordinates.x * TILE_SIZE),
    static_cast<float>(screenHeight - ((tileCoordinates.y + 1) * TILE_SIZE))
  };
}

void Level::resolveCollisions() {
  if (!player || !player->physicsBody) return;
  PhysicsBody& pb = *player->physicsBody;

  // Invisible left wall collision
  Rectangle playerBox = pb.getCollider(player->position);
  if (playerBox.x < 0) {
    player->position.x -= playerBox.x;
    pb.velocity.x = 0;
  }

  // TEMP: respawn player if they fall off the screen
  if (player->position.y - pb.getCollider(player->position).height > static_cast<float>(screenHeight) + 1000) {
    player->position = getPositionFromTileCoordinates({ 2, 5 }, screenWidth, screenHeight);
    pb.velocity = { 0, 0 };
    pb.acceleration = { 0, 0 };
    player->setJumping(false);
    player->setBoosting(false);
    player->setCanBoost(false);
  }

  // Check collisions with collidable entities
  pb.isGrounded = false;
  for (auto& entity : collidableEntities) {
    if (!entity->physicsBody) continue;

    Rectangle playerBox = pb.getCollider(player->position);
    Rectangle entityBox = entity->physicsBody->getCollider(entity->position);

    if (!CheckCollisionRecs(playerBox, entityBox)) continue;

    float overlapX = std::min(playerBox.x + playerBox.width,  entityBox.x + entityBox.width) - std::max(playerBox.x, entityBox.x);
    float overlapY = std::min(playerBox.y + playerBox.height, entityBox.y + entityBox.height) - std::max(playerBox.y, entityBox.y);

    if (overlapX < overlapY) { // Resolve horizontally
      if (playerBox.x < entityBox.x) {
        player->position.x -= overlapX;
      } else {
        player->position.x += overlapX;
      }
      pb.velocity.x = 0;
    } else { // Resolve vertically
      if (playerBox.y < entityBox.y) { // Player landed on top
        player->position.y -= overlapY;
        pb.velocity.y = 0;
        pb.isGrounded = true;
      } else { // Player hit ceiling
        player->position.y += overlapY;
        pb.velocity.y = 0;
      }
    }
  }
}

void Level::createBackgroundLayer(Texture2D texture) {
  float aspect = static_cast<float>(texture.width) / static_cast<float>(texture.height);
  float destHeight = static_cast<float>(screenHeight);
  float destWidth = destHeight * aspect;
  float x = (static_cast<float>(screenWidth) - destWidth) / 2.0f;

  staticEntities.push_back(std::make_unique<Entity>(
    Vector2{ x, 0 }, // top-left of layer
    Sprite(
      texture,
      Rectangle{ 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
      Vector2{ destWidth, destHeight },
      Vector2{ 0, 0 }
    )
  ));
}
void Level::createTile(Vector2 tileCoordinates, int frameIndex, bool enablePhysics) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  Entity tile = Entity(
    position,
    Sprite(
      &tilesSheet,
      frameIndex,
      Vector2{ TILE_SIZE, TILE_SIZE }, // size
      Vector2{ 0, 0 } // origin
    )
  );
  if (enablePhysics) {
    tile.enablePhysics(Vector2{ TILE_SIZE, TILE_SIZE }, Vector2{ 0, 0 }, true);
  }

  collidableEntities.push_back(std::make_unique<Entity>(tile));
}