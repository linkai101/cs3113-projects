#pragma once
#include "raylib.h"
#include "assets.h"
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

  std::unique_ptr<Scene> world;
  Scene* activeScene;

  void processInput();

  void update(float deltaTime);

  void render() const;

  void shutdown();
};