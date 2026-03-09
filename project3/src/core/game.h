#pragma once
#include "raylib.h"
#include "entities/player.h"
#include "core/level.h"
#include <memory>
#include <vector>

class Game {
public:
  Game(int width, int height, const char* title);
  
  ~Game();

  void init();

  void run();

private:
  int width;
  int height;
  const char* title;
  bool isRunning;

  std::vector<std::unique_ptr<Level>> levels;
  int currentLevelIndex;

  void processInput();

  void update(float deltaTime);

  void render();
};