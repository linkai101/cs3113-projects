#pragma once
#include "entities/entity.h"
#include "entities/player.h"
#include "components/follow_camera.h"
#include <vector>

class Scene {
public:
  Scene(
    int screenWidth, int screenHeight, Vector2 spawnPosition,
    Spritesheet& islandTerrainSheet
  );

  virtual ~Scene() = default;

  void load(Player* player);

  void unload();

  virtual void processInput(Player* player);

  void update(float deltaTime, Player* player);

  virtual void render(Player* player) const;

protected:
  int screenWidth, screenHeight;
  Vector2 spawnPosition;
  bool loaded = false;

  Spritesheet& islandTerrainSheet;

  std::vector<std::unique_ptr<Entity>> entities;
  
  std::vector<Entity*> foregroundEntities;
  std::vector<Entity*> backgroundEntities;

  FollowCamera camera;

  virtual void loadLevel() = 0;

  virtual void resetPlayer(Player* player);

  virtual void resolveCollisions(Player* player);

  void loadTileGrid(
    const int* grid, int rows, int cols,
    Spritesheet& sheet, Vector2 tileOffset,
    bool enablePhysics, bool foreground
  );

  static Vector2 getTilePosition(Vector2 tileCoordinates, Vector2 tileOffset = {0, 0});

  static constexpr int TILE_SIZE = 96;
  static constexpr int EMPTY_TILE = -1;
};