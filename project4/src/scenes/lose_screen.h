#pragma once
#include "scenes/scene.h"

class LoseScreen : public Scene {
public:
  LoseScreen(int screenWidth, int screenHeight, Assets& assets);

  void processInput(Player* player) override;

  void update(float deltaTime, Player* player) override;

  void render(Player* player) const override;

protected:
  void loadLevel() override {}
  void resolveCollisions(Player* player) override {}
};
