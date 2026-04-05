#include "scenes/scene.h"

Scene::Scene(
  int screenWidth, int screenHeight, Vector2 spawnPosition,
  Assets& assets
) :
  screenWidth(screenWidth),
  screenHeight(screenHeight),
  spawnPosition(spawnPosition),
  assets(assets),
  camera(screenWidth, screenHeight)
{}

void Scene::load(Player* player) {
  if (loaded) return;

  loadLevel();
  resetPlayer(player);
  camera.init(player->getPosition());

  loaded = true;
}

void Scene::unload() {
  if (!loaded) return;

  if (bgMusic) {
    StopMusicStream(*bgMusic);
    bgMusic = nullptr;
  }

  foregroundEntities.clear();
  backgroundEntities.clear();
  entities.clear();

  loaded = false;
}

void Scene::processInput(Player* player) {
  if (!loaded) return;
  
  player->processInput();
}

void Scene::update(float deltaTime, Player* player) {
  if (!loaded) return;

  for (auto& entity : entities) {
    entity->update(deltaTime);
  }

  player->update(deltaTime);
  resolveCollisions(player);

  camera.update(deltaTime, player->getPosition());
}

void Scene::render(Player* player) const {
  if (!loaded) return;

  BeginMode2D(camera.get());

  for (auto& entity : backgroundEntities) {
    entity->render();
  }

  player->render();

  for (auto& entity : foregroundEntities) {
    entity->render();
  }

  EndMode2D();

  // HUD
  const char* livesText = TextFormat("Lives: %d", player->getLives());
  DrawText(livesText, 10, 10, 20, WHITE);
}

void Scene::resetPlayer(Player* player) {
  player->getPosition() = spawnPosition;
  player->getPhysicsBody()->velocity = {0, 0};
  player->getPhysicsBody()->acceleration = {0, 0};
  player->getPhysicsBody()->isGrounded = false;
  player->setStunned(false);
  player->playAnimation("idle");
}

void Scene::loadTileGrid(
  const int* grid, int rows, int cols,
  Spritesheet& sheet, Vector2 tileOffset,
  bool enablePhysics, bool foreground
) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int frameIndex = grid[r * cols + c];
      if (frameIndex == EMPTY_TILE) continue;

      Vector2 position = getTilePosition({static_cast<float>(c), static_cast<float>(r)}, tileOffset);

      Entity tile = Entity(
        position,
        Sprite(
          &sheet,
          frameIndex,
          Vector2{ TILE_SIZE, TILE_SIZE },
          Vector2{ 0, 0 }
        )
      );
      if (enablePhysics) tile.enablePhysics(Vector2{ TILE_SIZE, TILE_SIZE }, Vector2{ 0, 0 }, true);

      // Add to scene
      entities.push_back(std::make_unique<Entity>(tile));
      if (foreground) foregroundEntities.push_back(entities.back().get());
      else backgroundEntities.push_back(entities.back().get());
      if (enablePhysics) terrainEntities.push_back(entities.back().get());
    }
  }
}

Vector2 Scene::getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset) {
  return {
    static_cast<float>((tileOffset.x + tileCoordinates.x) * TILE_SIZE),
    static_cast<float>((tileOffset.y + tileCoordinates.y) * TILE_SIZE)
  };
}