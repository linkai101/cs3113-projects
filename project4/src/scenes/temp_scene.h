#pragma once
#include "scene.h"

class TempScene : public Scene {
public:
  TempScene(
    int screenWidth, int screenHeight,
    Spritesheet& islandTerrainSheet
  );

  void resolveCollisions(Player* player) override;

  void render(Player* player) const override;

protected:
  void loadLevel() override;

private:
  static constexpr int MAP_ROWS = 5;
  static constexpr int MAP_COLS = 10;
  static constexpr int MAP[MAP_ROWS][MAP_COLS] = {
    { 18, 6, 18, 18, 18, 18, 18, 18, 18, 18 },
    { 6, 36, -1, -1, -1, -1, -1, -1, -1, 18 },
    { 18, -1, -1, -1, -1, -1, -1, -1, -1, 18 },
    { 18, -1, -1, -1, -1, -1, -1, -1, -1, 18 },
    { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
  };
};