#pragma once
#include "entities/entity.h"
#include "entities/player.h"
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

  enum TileType {
    BLOCK = 0,
    INNER = 1,
    INNER_FOSSIL = 2,
    TOP_ALT = 3,
    TOP = 4,
    BOTTOM = 5,
    LEFT = 6,
    RIGHT = 7,
    TOP_LEFT = 8,
    TOP_RIGHT = 9,
    BOTTOM_LEFT = 10,
    BOTTOM_RIGHT = 11,
    CORNER_TOP_LEFT = 12,
    CORNER_TOP_RIGHT = 13,
    CORNER_BOTTOM_LEFT = 14,
    CORNER_BOTTOM_RIGHT = 15,
    SLOPE_TOP_LEFT = 16,
    SLOPE_TOP_RIGHT = 17,
    BRIDGE_LEFT = 18,
    BRIDGE_RIGHT = 19,
    BRIDGE_MIDDLE = 20
  };
};