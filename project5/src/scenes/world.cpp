#include <algorithm>
#include <cmath>
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

  if (
    IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
    player->canAttack()
  ) {
    player->attack();

    // Spawn bullets if attacking with a weapon
    std::optional<BulletType> bulletType = player->getEquippedBulletType();
    if (bulletType) spawnBullets(*bulletType, player->getAimAngle());
  }
  if (IsKeyPressed(KEY_R)) player->reload();

  // DEBUG
  if (IsKeyPressed(KEY_ONE)) player->debug(1); // equip hands
  if (IsKeyPressed(KEY_TWO)) player->debug(2); // equip bat
  if (IsKeyPressed(KEY_THREE)) player->debug(3); // equip rifle
  if (IsKeyPressed(KEY_FOUR)) player->debug(4); // equip pistol
  if (IsKeyPressed(KEY_FIVE)) player->debug(5); // equip shotgun

  if (IsKeyPressed(KEY_ZERO)) player->debug(0); // die

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

  // Remove expired bullets
  entities.erase(
    std::remove_if(entities.begin(), entities.end(),
      [](const std::unique_ptr<Entity>& e) {
        auto* b = dynamic_cast<Bullet*>(e.get());
        return b && b->isExpired();
      }),
    entities.end()
  );

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

void World::spawnBullets(BulletType type, float aimAngle) {
  // Spawn slightly ahead of the player in the aim direction
  static constexpr float SPAWN_DIST = 42.0f;
  static constexpr float WEAPON_Y_OFFSET = -30.0f;

  Vector2 origin = {
    player->getPosition().x + cosf(aimAngle) * SPAWN_DIST,
    player->getPosition().y + WEAPON_Y_OFFSET + sinf(aimAngle) * SPAWN_DIST
  };

  auto spawn = [this, origin, type](float angle) {
    entities.push_back(std::make_unique<Bullet>(origin, angle, type, assets));
  };

  if (type == BulletType::SHOTGUN) {
    static constexpr float SPREAD = 15.0f * DEG2RAD;
    spawn(aimAngle - SPREAD);
    spawn(aimAngle);
    spawn(aimAngle + SPREAD);
  } else {
    spawn(aimAngle);
  }
}