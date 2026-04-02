#pragma once
#include "scene.h"

class TempScene : public Scene {
public:
  TempScene(
    int screenWidth, int screenHeight,
    Spritesheet& tilesSheet
  );

  void resolveCollisions(Player* player) override;

  void render(Player* player) const override;

protected:
  void loadMap() override;
};