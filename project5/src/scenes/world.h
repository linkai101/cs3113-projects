#pragma once
#include "scenes/scene.h"
#include "entities/player.h"
#include "entities/bullet.h"
#include "entities/dummy.h"
#include "entities/zombie.h"

class World : public Scene {
public:
  World(int screenWidth, int screenHeight, Assets& assets);

  void load() override;

  void unload() override;

  void processInput() override;

  void update(float deltaTime) override;

  void render() const override;

private:
  FollowCamera camera;

  std::vector<std::unique_ptr<Entity>> terrain;
  std::vector<std::unique_ptr<Entity>> entities;

  Player* player = nullptr;
  Dummy* dummy = nullptr;
  Zombie* zombie = nullptr;
  bool playerMeleeHitRegistered = false;

  void spawnBullets(Gun::Type type, Gun::Properties properties, float aimAngle, int bulletCount, float spread);

  void loadTileGrid(
    std::vector<std::unique_ptr<Entity>>& vectorToAddTo,
    const int* grid, int rows, int cols,
    Spritesheet& sheet, Vector2 tileOffset,
    bool enablePhysics
  );

  static Vector2 getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset = {0, 0});

  static constexpr int TILE_SIZE = 80;
  static constexpr int EMPTY_TILE = -1;

  static constexpr int MAP_ROWS = 5;
  static constexpr int MAP_COLS = 5;
  static constexpr int TERRAIN_MAP[MAP_ROWS][MAP_COLS] = {
    { 5, 5, 5, 5, 5 },
    { 5, 5, 5, 5, 5 },
    { 5, 5, 5, 5, 5 },
    { 5, 5, 5, 5, 5 },
    { 5, 5, 5, 5, 5 },
  };
  static constexpr Vector2 SPAWN_POSITION = {2.5f, 2.5f};

  static constexpr float BULLET_SPAWN_DIST = 42.0f;
  static constexpr float BULLET_SPAWN_Y_OFFSET = -42.0f;
};