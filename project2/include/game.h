#pragma once
#include "raylib.h"

class Game {
public:
  Game(int width, int height, const char* title);
  ~Game();

  /**
   * Runs the game loop
   */
  void run();

private:
  int width;
  int height;
  const char* title;
  bool isRunning;
  
  void init();

  void processInput();

  void update(float deltaTime);

  void render();
  
  void shutdown();
};