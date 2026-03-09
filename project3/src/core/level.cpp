#include "core/level.h"
#include "components/sprite.h"
#include "components/spritesheet.h"

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
  tilesSheet = Spritesheet{
    tilesTexture,
    Vector2{ 16, 16 }, // frame size
    4 // columns
  };
  playerTexture = LoadTexture("assets/textures/jetpackman.png");
  playerSheet = Spritesheet{
    playerTexture,
    Vector2{ 692, 599 }, // frame size
    5 // columns
  };

  // Background
  environment.push_back(std::make_unique<Entity>(
    Vector2{ 0, 0 }, // position
    Sprite(
      skyTexture,
      Rectangle{ 0, 0, static_cast<float>(skyTexture.width), static_cast<float>(skyTexture.height) }, // texture area
      Vector2{ static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
      Vector2{ 0, 0 } // origin
    )
  ));
  environment.push_back(std::make_unique<Entity>(
    Vector2{ 0, 0 }, // position
    Sprite(
      mountains1Texture,
      Rectangle{ 0, 0, static_cast<float>(mountains1Texture.width), static_cast<float>(mountains1Texture.height) }, // texture area
      Vector2{ static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
      Vector2{ 0, 0 } // origin
    )
  ));
  environment.push_back(std::make_unique<Entity>(
    Vector2{ 0, 0 }, // position
    Sprite(
      mountains2Texture,
      Rectangle{ 0, 0, static_cast<float>(mountains2Texture.width), static_cast<float>(mountains2Texture.height) }, // texture area
      Vector2{ static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
      Vector2{ 0, 0 } // origin
    )
  ));

  // Create environment entities
  createTile({ 0, 0 }, TileType::RIGHT);
  createTile({ 0, 1 }, TileType::RIGHT);
  createTile({ 0, 2 }, TileType::CORNER_BOTTOM_RIGHT);
  createTile({ 1, 2 }, TileType::BOTTOM);
  createTile({ 2, 2 }, TileType::BOTTOM);
  createTile({ 3, 2 }, TileType::BOTTOM);
  createTile({ 4, 2 }, TileType::BOTTOM_RIGHT);
  createTile({ 0, 3 }, TileType::TOP);
  createTile({ 1, 3 }, TileType::TOP);
  createTile({ 2, 3 }, TileType::TOP_ALT);
  createTile({ 3, 3 }, TileType::TOP);
  createTile({ 4, 3 }, TileType::TOP_RIGHT);
  createTile({ 1, 1 }, TileType::SLOPE_TOP_LEFT);

  // Create player
  // const std::map<int, std::vector<int>> Player::ANIMATION_ATLAS = {
  //   {STANDING_IDLE, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }},
  //   {STANDING_WALK, { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }},
  //   {STANDING_RUN, { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }},
  //   {STANDING_JUMP, { 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 }},
  //   {STANDING_DIE, { 55, 56, 57, 58, 59 }},
  //   {FLYING, { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 }},
  //   {FLYING_NO_MOVEMENT, { 75, 76, 77, 78, 79, 80, 81, 82, 83, 84 }},
  //   {FLYING_DIE, { 85, 86, 87, 88, 89 }}
  // };
  Animator playerAnimator = Animator(
    &playerSheet,
    Vector2{ 150, 130 }, // size
    Vector2{ 90, 115 } // origin
  );
  int playerAnimatorFPS = 20;
  playerAnimator.addAnimation("standing_idle", Animator::Animation{"standing_idle", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, playerAnimatorFPS, true});
  playerAnimator.addAnimation("standing_walk", Animator::Animation{"standing_walk", { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }, playerAnimatorFPS, true});
  playerAnimator.addAnimation("standing_run", Animator::Animation{"standing_run", { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }, playerAnimatorFPS, true});
  playerAnimator.addAnimation("standing_jump", Animator::Animation{"standing_jump", { 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 }, playerAnimatorFPS, false});
  playerAnimator.addAnimation("standing_die", Animator::Animation{"standing_die", { 55, 56, 57, 58, 59 }, playerAnimatorFPS, false});
  playerAnimator.addAnimation("flying", Animator::Animation{"flying", { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 }, playerAnimatorFPS, true});
  playerAnimator.addAnimation("flying_no_movement", Animator::Animation{"flying_no_movement", { 75, 76, 77, 78, 79, 80, 81, 82, 83, 84 }, playerAnimatorFPS, true});
  playerAnimator.addAnimation("flying_die", Animator::Animation{"flying_die", { 85, 86, 87, 88, 89 }, playerAnimatorFPS, false});

  player = std::make_unique<Player>(
    getPositionFromTileCoordinates({ 2, 3 }, screenWidth, screenHeight),
    playerAnimator
  );
  player->playAnimation("standing_run");
}

Level::~Level() {}

void Level::processInput() {}

void Level::update(float deltaTime) {
  for (auto& entity : environment) {
    entity->update(deltaTime);
  }

  if (player) player->update(deltaTime);
}

void Level::render() const {
  for (auto& entity : environment) {
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

void Level::createTile(Vector2 tileCoordinates, int frameIndex) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  environment.push_back(std::make_unique<Entity>(
    position,
    Sprite(
      &tilesSheet,
      frameIndex,
      Vector2{ TILE_SIZE, TILE_SIZE }, // size
      Vector2{ 0, 0 } // origin
    )
  ));
}