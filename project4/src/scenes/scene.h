#pragma once
#include "entities/entity.h"
#include "entities/player.h"
#include "components/follow_camera.h"
#include <vector>

class Scene {
public:
  Scene(
    int screenWidth, int screenHeight, Vector2 spawnPosition,
    Spritesheet& islandTerrainSheet, Spritesheet& smallMapSheet
  );

  virtual ~Scene() = default;

  void load(Player* player);

  void unload();

  virtual void processInput(Player* player);

  void update(float deltaTime, Player* player);

  void render(Player* player) const;

  bool isTransitionRequested() const { return transitionRequested; }
  void clearTransition() { transitionRequested = false; }

protected:
  int screenWidth, screenHeight;
  Vector2 spawnPosition;
  bool loaded = false;
  bool transitionRequested = false;

  Spritesheet& islandTerrainSheet;
  Spritesheet& smallMapSheet;

  std::vector<std::unique_ptr<Entity>> entities; // all unique entities stored here

  std::vector<Entity*> foregroundEntities;
  std::vector<Entity*> backgroundEntities;

  FollowCamera camera;

  void requestTransition() { transitionRequested = true; }

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