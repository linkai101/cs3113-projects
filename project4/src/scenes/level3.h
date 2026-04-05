#pragma once
#include "scenes/scene.h"
#include "entities/star.h"

class Level3 : public Scene {
public:
  Level3(int screenWidth, int screenHeight, Assets& assets);

protected:
  void loadLevel() override;

  void resolveCollisions(Player* player) override;

public:
  void update(float deltaTime, Player* player) override;

private:
  Entity* levelGoal;
  Star* star;
  
  static constexpr int E = EMPTY_TILE;
  static constexpr int MAP_ROWS = 8;
  static constexpr int MAP_COLS = 14;
  static constexpr int TERRAIN_MAP[MAP_ROWS][MAP_COLS] = {
    { 6, 35, 35, 35, 35,  35, 35, 35, 35, 35, 35, 35, 7, 18 },
    { 19, E, E, E, E, E, E, E, E, E, E, E, 34, 7 },
    { 19, E, E, E, E, E, E, E, E, E, E, E, E, 17 },
    { 19, E, E, E, E, E, 0, 2, E, 0, 2, E, E, 17 },
    { 23, 60, 69, 69, 69, 69, 78, 36, E, 17, 19, E, E, 17 },
    { 18, 19, E, E, E, E, E, E, E, 34, 36, E, E, 17 },
    { 18, 19, E, E, E, 4, E, E, E, E, E, E, E, 17 },
    { 18, 23, 1, 1, 1, 9, 1, 1,  1, 1, 1, 1, 1, 24 },
  };
  static constexpr int PLATFORM_MAP[MAP_ROWS][MAP_COLS] = {
    { E, E, E, E, E, E, E, E, E, E, E, E, E, E },
    { E, E, E, E, E, E, E, E, E, E, E, E, E, E },
    { E, E, E, E, E, E, E, E, E, E, E, E, E, E },
    { E, E, E, E, E, E, E, E, E, E, E, 6, E, E },
    { E, E, E, E, E, E, E, E, E, E, E, E, 4, E },
    { E, E, E, E, E, E, E, E, E, E, E, 6, E, E },
    { E, E, E, E, E, E, E, E, E, E, E, E, 4, E },
    { E, E, E, E, E, E, E, E, E, E, E, E, E, E },
  };
  static constexpr Vector2 SPAWN_POSITION = {2.5f, 6.5f};
  static constexpr Vector2 GOAL_SPAWN_POSITION = {1.5f, 3.5f};
};