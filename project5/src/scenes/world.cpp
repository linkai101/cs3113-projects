#include <algorithm>
#include "scenes/world.h"

World::World(int screenWidth, int screenHeight, Assets& assets) :
  Scene(screenWidth, screenHeight, assets),
  camera(screenWidth, screenHeight)
{}

void World::load() {
  if (loaded) return;

  loadTileGrid(
    terrain,
    &TERRAIN_MAP[0][0], MAP_ROWS, MAP_COLS,
    assets.terrainSheet, Vector2{0, 0},
    false
  );
  
  entities.push_back(std::make_unique<Player>(getTilePosition(SPAWN_POSITION), assets));
  player = dynamic_cast<Player*>(entities.back().get());

  camera.init(player->getPosition());

  Scene::load();
}

void World::unload() {
  if (!loaded) return;
  
  entities.clear();

  Scene::unload();
}

void World::processInput() {
  if (!loaded) return;

  bool up = IsKeyDown(KEY_W);
  bool down = IsKeyDown(KEY_S);
  bool left = IsKeyDown(KEY_A);
  bool right = IsKeyDown(KEY_D);

  // DEBUG
  if (IsKeyPressed(KEY_ONE)) player->debug(1);
  if (IsKeyPressed(KEY_TWO)) player->debug(2);
  if (IsKeyPressed(KEY_THREE)) player->debug(3);
  if (IsKeyPressed(KEY_FOUR)) player->debug(4);
  if (IsKeyPressed(KEY_FIVE)) player->debug(5);

  if (IsKeyPressed(KEY_SEVEN)) player->debug(7);
  if (IsKeyPressed(KEY_EIGHT)) player->debug(8);
  if (IsKeyPressed(KEY_NINE)) player->debug(9);
  if (IsKeyPressed(KEY_ZERO)) player->debug(0);

  player->move(up, down, left, right);
}

void World::update(float deltaTime) {
  if (!loaded) return;

  if (player) {
    player->setMouseWorldPosition(GetScreenToWorld2D(GetMousePosition(), camera.get()));
  }

  for (auto& entity : entities) {
    entity->update(deltaTime);
  }

  if (player) {
    camera.update(deltaTime, player->getPosition());
  }
}

void World::render() const {
  if (!loaded) return;

  BeginMode2D(camera.get());

  // Render terrain entities
  for (auto& entity : terrain) {
    entity->render();
  }

  // Render entities by y position
  std::vector<Entity*> sorted;
  sorted.reserve(entities.size());
  for (auto& entity : entities) sorted.push_back(entity.get());
  std::sort(sorted.begin(), sorted.end(), [](const Entity* a, const Entity* b) {
    return a->getPosition().y < b->getPosition().y;
  });
  for (auto* entity : sorted) entity->render();

  EndMode2D();
}

void World::loadTileGrid(
  std::vector<std::unique_ptr<Entity>>& vectorToAddTo,
  const int* grid, int rows, int cols,
  Spritesheet& sheet, Vector2 tileOffset,
  bool enablePhysics
) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      int frameIndex = grid[r * cols + c];
      if (frameIndex == -1) continue;

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
      if (enablePhysics) {
        tile.enablePhysics(Vector2{ TILE_SIZE, TILE_SIZE }, Vector2{ 0, 0 }, true);
      }

      // Add to scene
      vectorToAddTo.push_back(std::make_unique<Entity>(tile));
    }
  }
}

Vector2 World::getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset) {
  return {
    static_cast<float>((tileOffset.x + tileCoordinates.x) * TILE_SIZE),
    static_cast<float>((tileOffset.y + tileCoordinates.y) * TILE_SIZE)
  };
}