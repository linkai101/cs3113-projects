#pragma once
#include <string>
#include <vector>
#include "scenes/scene.h"
#include "entities/player.h"
#include "entities/dummy.h"
#include "entities/bullet.h"
#include "items/ammo_crate.h"
#include "items/bandage.h"
#include "hud/hotbar.h"

class Tutorial : public Scene {
public:
  Tutorial(int screenWidth, int screenHeight, Assets& assets);

  void load() override;

  void unload() override;

  void processInput() override;

  void update(float deltaTime) override;
  
  void render() const override;

  void renderHUD() const override;

  float getPlayerDamageFlashIntensity() const override;
  
  SceneTransition getTransition() const override { return transition; }

private:
  int mapCols = 0;
  int mapRows = 0;
  SceneTransition transition = SceneTransition::NONE;

  FollowCamera camera;
  Hotbar hotbar;

  std::vector<std::unique_ptr<Entity>> terrain;
  std::vector<Rectangle> collisionBoxes;
  std::vector<std::unique_ptr<Entity>> entities;
  std::vector<std::unique_ptr<Bullet>> bullets;

  Player* player = nullptr;
  Dummy* dummy = nullptr;
  std::vector<std::unique_ptr<AmmoCrate>> ammoCrates;
  std::vector<std::unique_ptr<Bandage>> bandages;
  bool playerMeleeHitRegistered = false;

  void spawnBullets(Gun::Type type, Gun::Properties properties, int bulletCount, float spread);

  void loadLevel(const std::string& path);
  void loadLayerFromGrid(const std::string& layerName, const std::vector<std::vector<int>>& grid);
  void loadTileGrid(
    std::vector<std::unique_ptr<Entity>>& vec,
    const int* grid, int rows, int cols,
    Spritesheet& sheet, Vector2 tileOffset, bool enablePhysics
  );
  static Vector2 getTilePosition(Vector2 tileCoords, Vector2 tileOffset = {0, 0});

  static constexpr Vector2 CURSOR_RENDER_SIZE = {25, 35};
  static constexpr int TILE_SIZE = 80;
  static constexpr Vector2 SPAWN_POSITION = {4.5f, 3.5f};
  static constexpr Vector2 DUMMY_POSITION = {8.0f, 1.5f};
  static constexpr float BULLET_SPAWN_DIST = 42.0f;
  static constexpr float BULLET_SPAWN_Y_OFFSET = -42.0f;
};
