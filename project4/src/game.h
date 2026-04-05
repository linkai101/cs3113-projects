#pragma once
#include "raylib.h"
#include "assets.h"
#include "entities/player.h"
#include "scenes/scene.h"
#include <memory>

class Game {
public:
  Game(int width, int height, const char* title);

  void init();

  void run();

private:
  int width;
  int height;
  const char* title;
  bool isRunning;
  float gPreviousTicks = 0.0f; // for calculating deltaTime

  Assets assets;

  std::unique_ptr<Scene> mainMenu;
  std::unique_ptr<Scene> level1;
  std::unique_ptr<Scene> level2;
  std::unique_ptr<Scene> level3;
  std::unique_ptr<Scene> winScreen;
  std::unique_ptr<Scene> loseScreen;
  Scene* activeScene;

  std::unique_ptr<Player> player;

  void processInput();

  void update(float deltaTime);

  void render() const;

  void resetGame();

  void shutdown();
};