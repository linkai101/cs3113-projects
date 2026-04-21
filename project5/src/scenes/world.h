#pragma once
#include <string>
#include <vector>
#include "scenes/scene.h"
#include "entities/player.h"
#include "entities/bullet.h"
#include "entities/ghoul_axe.h"
#include "entities/enemy/enemy.h"
#include "entities/enemy/ghoul.h"
#include "items/ammo_crate.h"
#include "items/bandage.h"
#include "hud/hotbar.h"

class World : public Scene {
public:
  World(int screenWidth, int screenHeight, Assets& assets, std::string levelPath);

  void load() override;

  void unload() override;

  void processInput() override;

  void update(float deltaTime) override;

  void render() const override;

  void renderHUD() const override;

  float getPlayerDamageFlashIntensity() const override;

private:
  enum class GameState { WAITING, IN_WAVE, WIN, GAME_OVER };
  enum class EnemyType { ZOMBIE, GIANT, GHOUL };

  struct WaveEnemyCounts { int zombies, giants, ghouls; };
  struct WaveItemCounts  { int rifleCrates, pistolCrates, shotgunCrates, bandages; };

  int mapCols = 0;
  int mapRows = 0;

  FollowCamera camera;
  Hotbar hotbar;
  std::string levelPath;

  std::vector<std::unique_ptr<Entity>> terrain;
  std::vector<Rectangle> collisionBoxes;
  std::vector<std::unique_ptr<Entity>> entities;
  std::vector<std::unique_ptr<Bullet>> bullets;
  std::vector<std::unique_ptr<GhoulAxe>> ghoulAxes;

  Player* player = nullptr;
  std::vector<Enemy*> enemies;
  std::vector<Ghoul*> ghouls;
  std::vector<std::unique_ptr<AmmoCrate>> ammoCrates;
  std::vector<std::unique_ptr<Bandage>> bandages;
  bool playerMeleeHitRegistered = false;

  GameState gameState = GameState::WAITING;
  int currentWave = 0;
  float stateTimer = 0.0f;
  float enemySpawnTimer = 0.0f;
  std::vector<EnemyType> spawnQueue;
  int spawnQueuePos = 0;

  void startWave();
  void spawnItems();
  void spawnEnemy(EnemyType type, Vector2 position);
  Vector2 findSpawnPosition(float minDistFromPlayer);

  void spawnBullets(Gun::Type type, Gun::Properties properties, float aimAngle, int bulletCount, float spread);

  void loadLevel(const std::string& path);
  void loadLayerFromGrid(const std::string& layerName, const std::vector<std::vector<int>>& grid);
  void loadTileGrid(
    std::vector<std::unique_ptr<Entity>>& vectorToAddTo,
    const int* grid, int rows, int cols,
    Spritesheet& sheet, Vector2 tileOffset,
    bool enablePhysics
  );

  static Vector2 getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset = {0, 0});

  static constexpr Vector2 CURSOR_RENDER_SIZE = {25, 35};
  static constexpr int TILE_SIZE = 80;
  static constexpr Vector2 SPAWN_POSITION = {9.5f, 5.5f};
  static constexpr float BULLET_SPAWN_DIST = 42.0f;
  static constexpr float BULLET_SPAWN_Y_OFFSET = -42.0f;

  // Wave system
  static constexpr int NUM_WAVES = 3;
  static constexpr WaveEnemyCounts WAVE_ENEMY_COUNTS[NUM_WAVES] = {
    {10, 0, 0}, // wave 1
    {10, 5, 0}, // wave 2
    {12, 2, 6}, // wave 3
  };
  static constexpr float WAVE_START_DELAY = 10.0f;
  static constexpr float WAVE_BETWEEN_DELAY = 10.0f;
  static constexpr float ENEMY_SPAWN_INTERVAL = 1.5f;
  static constexpr float MIN_ENEMY_SPAWN_DIST = 300.0f;
  static constexpr int MAX_SPAWN_ATTEMPTS = 50;

  // Enemy/item spawn area
  static constexpr float SPAWN_AREA_X = 0.0f;
  static constexpr float SPAWN_AREA_Y = 3.0f * TILE_SIZE;
  static constexpr float SPAWN_AREA_W = 24.0f * TILE_SIZE;
  static constexpr float SPAWN_AREA_H = 7.0f * TILE_SIZE;

  // Items spawned before each wave
  static constexpr WaveItemCounts WAVE_ITEM_COUNTS[NUM_WAVES] = {
    {0, 0, 0, 0}, // before wave 1
    {0, 1, 1, 1}, // before wave 2
    {1, 0, 1, 2}, // before wave 3
  };
  static constexpr int AMMO_RIFLE_PER_CRATE = 15;
  static constexpr int AMMO_PISTOL_PER_CRATE = 12;
  static constexpr int AMMO_SHOTGUN_PER_CRATE = 6;
};
