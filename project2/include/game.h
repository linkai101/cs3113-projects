#pragma once
#include "raylib.h"
#include "player.h"
#include "ball.h"

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
  enum BallCount { ONE_BALL = 1, TWO_BALLS = 2, THREE_BALLS = 3 };

  int width;
  int height;
  const char* title;
  bool isRunning;
  GameState gameState;

  GameMode gameMode;
  BallCount ballCount;
  int player1Score;
  int player2Score;

  Entity* wall;
  Player* player1;
  Player* player2;
  Ball* balls[3];
  float serveTimers[3]; // countdown before each ball is served (seconds)
  
  void init();

  void processInput();

  void update(float deltaTime);

  void render();
  
  void shutdown();

  void checkGameOver();

  void checkCollisions();

  static constexpr int HEADING_FONT_SIZE = 40;
  static constexpr int PARAGRAPH_FONT_SIZE = 20;
  static constexpr int SCORE_FONT_SIZE = 30;
  static constexpr int WIN_SCORE = 10; // Number of points to win
  static constexpr float SERVE_DELAY = 3.0f; // Seconds between each ball being served
};