#pragma once
#include "scenes/scene.h"

class MainMenu : public Scene {
public:
  MainMenu(int screenWidth, int screenHeight, Assets& assets);

  void load() override;

  void unload() override;

  void processInput() override;

  void update(float deltaTime) override;

  void render() const override;

  void renderHUD() const override;
  
  SceneTransition getTransition() const override { return transition; }

private:
  SceneTransition transition = SceneTransition::NONE;
};
