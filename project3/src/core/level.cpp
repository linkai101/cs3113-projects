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

  grassTexture = LoadTexture("assets/textures/grass.png");
  grassSheet = Spritesheet{grassTexture, Vector2{ 16, 20 }, 9};

  waterTexture = LoadTexture("assets/textures/water.png");
  waterSheet = Spritesheet{waterTexture, Vector2{ 16, 16 }, 6};

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

  for (auto& entity : staticEntities) {
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

  DrawText(TextFormat("FPS: %d", GetFPS()), 5, 5, 15, WHITE);

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

  if (player) player->render();
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

  // Check collisions with collidable entities — two passes to separate axes.
  pb.isGrounded = false;
  // Resolve horizontal collisions
  for (auto& entity : collidableEntities) {
    if (!entity->physicsBody) continue;

    Rectangle playerBox = pb.getCollider(player->position);
    Rectangle entityBox = entity->physicsBody->getCollider(entity->position);

    if (!CheckCollisionRecs(playerBox, entityBox)) continue;

    float overlapX = std::min(playerBox.x + playerBox.width,  entityBox.x + entityBox.width) - std::max(playerBox.x, entityBox.x);
    float overlapY = std::min(playerBox.y + playerBox.height, entityBox.y + entityBox.height) - std::max(playerBox.y, entityBox.y);

    if (overlapX < overlapY) {
      if (playerBox.x < entityBox.x) {
        player->position.x -= overlapX;
      } else {
        player->position.x += overlapX;
      }
      pb.velocity.x = 0;
    }
  }
  // Resolve vertical collisions
  for (auto& entity : collidableEntities) {
    if (!entity->physicsBody) continue;

    Rectangle playerBox = pb.getCollider(player->position);
    Rectangle entityBox = entity->physicsBody->getCollider(entity->position);

    if (!CheckCollisionRecs(playerBox, entityBox)) continue;

    float overlapX = std::min(playerBox.x + playerBox.width,  entityBox.x + entityBox.width) - std::max(playerBox.x, entityBox.x);
    float overlapY = std::min(playerBox.y + playerBox.height, entityBox.y + entityBox.height) - std::max(playerBox.y, entityBox.y);

    if (overlapY <= overlapX) {
      if (playerBox.y < entityBox.y) { // Player landed on top
        player->position.y -= overlapY;
        pb.isGrounded = true;
        pb.velocity.y = 0;
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

void Level::createGrass(Vector2 tileCoordinates, int frameIndex, bool hanging) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);
  if (!hanging) position.y -= TILE_SIZE / 16 * (20 - 16);

  staticEntities.push_back(std::make_unique<Entity>(
    position,
    Sprite(
      &grassSheet,
      frameIndex,
      Vector2{ TILE_SIZE, TILE_SIZE / 16 * 20 }, // size
      Vector2{ 0, 0 } // origin
    )
  ));
}

void Level::createWater(Vector2 tileCoordinates, bool surface) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  if (surface) {
    Animator animator = Animator(
      &waterSheet,
      Vector2{ TILE_SIZE, TILE_SIZE }, // size
      Vector2{ 0, 0 } // origin
    );
    animator.addAnimation("surface", Animator::Animation{"surface", { 1, 2, 3, 4, 5 }, 10, true});
    animator.play("surface");

    staticEntities.push_back(std::make_unique<Entity>(
      position,
      animator
    ));
  } else {
    staticEntities.push_back(std::make_unique<Entity>(
      position,
      Sprite(
        &waterSheet,
        0,
        Vector2{ TILE_SIZE, TILE_SIZE }, // size
        Vector2{ 0, 0 } // origin
      )
    ));
  }
}

Vector2 Level::getPositionFromTileCoordinates(Vector2 tileCoordinates, int screenWidth, int screenHeight) {
  return {
    static_cast<float>(tileCoordinates.x * TILE_SIZE),
    static_cast<float>(screenHeight - ((tileCoordinates.y + 1) * TILE_SIZE))
  };
}