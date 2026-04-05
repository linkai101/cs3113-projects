#pragma once
#include "scenes/scene.h"

class WinScreen : public Scene {
public:
  WinScreen(int screenWidth, int screenHeight, Assets& assets);

  void processInput(Player* player) override;

  void update(float deltaTime, Player* player) override;

  void render(Player* player) const override;

protected:
  void loadLevel() override {}
  void resolveCollisions(Player* player) override {}
};
