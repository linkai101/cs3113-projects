#include "level.h"

Vector2 Level::getPositionFromTileCoordinates(Vector2 tileCoordinates, int screenWidth, int screenHeight) {
  return {
    static_cast<float>(tileCoordinates.x * TILE_SIZE),
    static_cast<float>(screenHeight - ((tileCoordinates.y + 1) * TILE_SIZE))
  };
}

void Level::createTile(Vector2 tileCoordinates, int textureSpriteIndex) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  environment.push_back(std::make_unique<Entity>(
    position,
    Vector2{ TILE_SIZE, TILE_SIZE }, // size
    Vector2{ 0, 0 }, // origin
    "assets/textures/tiles.png", // textureFilePath
    Vector2{ 16, 16 }, // textureSpriteSize
    4, // textureSpriteColumns
    textureSpriteIndex
  ));
}

Level::Level(
  int screenWidth,
  int screenHeight
) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  player()
{
  // Background
  environment.push_back(std::make_unique<Entity>(
    Vector2{ 0, 0 }, // position
    Vector2{ static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
    Vector2{ 0, 0 }, // origin
    "assets/textures/sky.png"
  ));
  environment.push_back(std::make_unique<Entity>(
    Vector2{ 0, 0 }, // position
    Vector2{ static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
    Vector2{ 0, 0 }, // origin
    "assets/textures/mountains_1.png"
  ));
  environment.push_back(std::make_unique<Entity>(
    Vector2{ 0, 0 }, // position
    Vector2{ static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
    Vector2{ 0, 0 }, // origin
    "assets/textures/mountains_2.png"
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

  player = std::make_unique<Player>(
    getPositionFromTileCoordinates({ 2, 3 }, screenWidth, screenHeight)
  );
}

Level::~Level() {}

void Level::processInput() {}

void Level::update(float deltaTime) {
  for (auto& entity : environment) {
    entity->update(deltaTime);
  }

  player->update(deltaTime);
}

void Level::render() const {
  for (auto& entity : environment) {
    entity->render();
  }

  player->render();
}