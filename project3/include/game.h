#pragma once
#include "raylib.h"
#include "player.h"
#include "level.h"

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

  std::vector<Level*> levels;
  int currentLevelIndex;

  void processInput();

  void update(float deltaTime);

  void render();
  
  void shutdown();
};