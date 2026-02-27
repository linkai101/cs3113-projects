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
   * Runs the game loop
   */
  void run();

private:
  enum GameState { MENU, PLAYING, GAME_OVER };

  enum GameMode { ONE_PLAYER, TWO_PLAYER };
  enum BallCount { ONE_BALL, TWO_BALLS, THREE_BALLS };

  int width;
  int height;
  const char* title;
  bool isRunning;
  GameState gameState;

  GameMode gameMode;
  BallCount ballCount;
  
  void init();

  void processInput();

  void update(float deltaTime);

  void render();
  
  void shutdown();
};