#pragma once
#include "raylib.h"

class Game {
public:
  Game(
    int width,
    int height,
    const char* title
  );
  ~Game();

  /**
   * Initializes the game.
   */
  void init();

  /**
   * Runs the game loop
   */
  void run();

private:
  int width;
  int height;
  const char* title;
  bool isRunning;

  void processInput();

  void update(float deltaTime);

  void render();
  
  void shutdown();
};