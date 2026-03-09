#include "level.h"

Vector2 Level::getPositionFromTileCoordinates(Vector2 tileCoordinates, int screenWidth, int screenHeight) {
  return {
    static_cast<float>(tileCoordinates.x * TILE_SIZE),
    static_cast<float>(screenHeight - ((tileCoordinates.y + 1) * TILE_SIZE))
  };
}

void Level::createTile(Vector2 tileCoordinates, int textureSpriteIndex) {
  Vector2 position = getPositionFromTileCoordinates(tileCoordinates, screenWidth, screenHeight);

  environment.push_back(new Entity(
    position,
    { TILE_SIZE, TILE_SIZE }, // size
    { 0, 0 }, // origin
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/tiles.png", // textureFilePath
    { 16, 16 }, // textureSpriteSize
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
  player(nullptr)
{
  // Background
  environment.push_back(new Entity(
    { 0, 0 }, // position
    { static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
    { 0, 0 }, // origin
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/sky.png"
  ));
  environment.push_back(new Entity(
    { 0, 0 }, // position
    { static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
    { 0, 0 }, // origin
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/mountains_1.png"
  ));
  environment.push_back(new Entity(
    { 0, 0 }, // position
    { static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, // size
    { 0, 0 }, // origin
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/mountains_2.png"
  ));

  // Create environment entities
  createTile({ 0, 0 }, 7);
  createTile({ 0, 1 }, 7);
  createTile({ 0, 2 }, 15);
  createTile({ 1, 2 }, 5);
  createTile({ 2, 2 }, 5);
  createTile({ 3, 2 }, 5);
  createTile({ 4, 2 }, 11);
  createTile({ 0, 3 }, 4);
  createTile({ 1, 3 }, 4);
  createTile({ 2, 3 }, 3);
  createTile({ 3, 3 }, 4);
  createTile({ 4, 3 }, 9);
  createTile({ 1, 1 }, 16); // slope


  player = new Player(
    getPositionFromTileCoordinates({ 2, 3 }, screenWidth, screenHeight)
  );
}

Level::~Level() {
  if (player) {
    delete player;
    player = nullptr;
  }

  for (Entity* entity : environment) {
    delete entity;
  }
  environment.clear();
}

void Level::processInput() {}

void Level::update(float deltaTime) {
  for (Entity* entity : environment) {
    entity->update(deltaTime);
  }

  player->update(deltaTime);
}

void Level::render() {
  for (Entity* entity : environment) {
    entity->render();
  }

  player->render();
}