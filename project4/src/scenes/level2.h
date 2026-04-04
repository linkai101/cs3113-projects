#pragma once
#include "scenes/scene.h"
#include "entities/crabby.h"

class Level2 : public Scene {
public:
  Level2(int screenWidth, int screenHeight, Assets& assets);

protected:
  void loadLevel() override;

  void resolveCollisions(Player* player) override;

private:
  Entity* levelGoal;
  Crabby* crabby;
  
  static constexpr int E = EMPTY_TILE;
  static constexpr int MAP_ROWS = 9;
  static constexpr int MAP_COLS = 14;
  static constexpr int TERRAIN_MAP[MAP_ROWS][MAP_COLS] = {
    { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 6, 35, 7, 18 },
    { 6, 35, 35, 35, 35, 35, 27, 35, 35, 35, 36, E, 17, 18 },
    { 19, E, E, E, E, E, 38, E, E, E, E, E, 34, 7 },
    { 19, E, E, E, E, E, E, E, E, E, E, E, E, 17 },
    { 19, E, E, E, E, E, E, E, E, E, E, E, E, 17 },
    { 19, E, E, E, E, E, E, E, E, E, E, E, 0, 24 },
    { 19, E, E, E, 4, E, E, E, 0, 2, E, E, 17, 18 },
    { 19, E, E, 0, 9, 1, 1, 1, 24, 23, 1, 1, 24, 18 },
    { 23, 1, 1, 24, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
  };
  static constexpr Vector2 MAP_SPAWN_POSITION = {1.5f, 7.5f};
};