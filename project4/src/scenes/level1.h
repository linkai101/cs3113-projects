#pragma once
#include "scene.h"

class Level1 : public Scene {
public:
  Level1(
    int screenWidth, int screenHeight,
    Spritesheet& islandTerrainSheet
  );

  void resolveCollisions(Player* player) override;

  void render(Player* player) const override;

protected:
  void loadLevel() override;

private:
  static constexpr int MAP_ROWS = 8;
  static constexpr int MAP_COLS = 11;
  static constexpr int MAP[MAP_ROWS][MAP_COLS] = {
    { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
    { 18, 18, 6, 35, 27, 35, 35, 35, 35, 7, 18 },
    { 18, 6, 36, -1, 38, -1, -1, -1, -1, 17, 18 },
    { 18, 19, -1, -1, -1, -1, -1, -1, -1, 17, 18 },
    { 18, 19, -1, -1, -1, -1, -1, -1, -1, 17, 18 },
    { 18, 19, -1, -1, -1, -1, 0, 1, 1, 24, 18 },
    { 18, 23, 1, 1, 1, 1, 24, 18, 18, 18, 18 },
    { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
  };
  static constexpr Vector2 MAP_SPAWN_POSITION = {2.5f, 5.5f};
};