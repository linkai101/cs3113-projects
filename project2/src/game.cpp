#include "game.h"
#include "util/log.h"
#include "util/color.h"

Game::Game(
  int width,
  int height,
  const char* title
) : width(width), height(height), title(title), isRunning(true), gameState(MENU), gameMode(ONE_PLAYER), ballCount(ONE_BALL) {
  init();
}

Game::~Game() {
  shutdown();
}

void Game::run() {
  while (isRunning) {
    processInput();

    float deltaTime = GetFrameTime();
    update(deltaTime);

    render();
  }
}

void Game::init() {
  InitWindow(width, height, title);
  SetTargetFPS(60);
}

void Game::processInput() {
  if (WindowShouldClose()) {
    isRunning = false;
  }

  switch (gameState) {
    case MENU: {
      if (IsKeyPressed(KEY_T)) {
        gameMode = gameMode == ONE_PLAYER ? TWO_PLAYER : ONE_PLAYER;
      }
      if (IsKeyPressed(KEY_ONE)) {
        ballCount = ONE_BALL;
      }
      if (IsKeyPressed(KEY_TWO)) {
        ballCount = TWO_BALLS;
      }
      if (IsKeyPressed(KEY_THREE)) {
        ballCount = THREE_BALLS;
      }
      if (IsKeyPressed(KEY_SPACE)) {
        gameState = PLAYING;
      }

      break;
    }
    case PLAYING: {
      break;
    }
    case GAME_OVER: {
      break;
    }
  }
}

void Game::update(float deltaTime) {
  switch (gameState) {
    case MENU: {
      break;
    }
    case PLAYING: {
      break;
    }
      break;
    case GAME_OVER: {
      break;
    }
  }
}

void Game::render() {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));

  switch (gameState) {
    case MENU: {
      const int headingFontSize = 40;
      const int paragraphFontSize = 20;

      // Title
      const char* title = "Gravity Pong";
      const int titleWidth = MeasureText("Gravity Pong", headingFontSize);
      DrawText(
        "Gravity Pong",
        width/2 - titleWidth/2,
        height/2 - headingFontSize - 90,
        headingFontSize,
        ColorFromHex("#FFFFFF")
      );

      // Game Mode selector
      const char* modeTitle = "Game Mode (T)";  
      const int modeTitleWidth = MeasureText(modeTitle, paragraphFontSize);
      DrawText(
        modeTitle,
        width/2 - 175,
        height/2 - paragraphFontSize/2 - 25,
        paragraphFontSize,
        ColorFromHex("#FFFFFF")
      );

      const char* modeValue = gameMode == ONE_PLAYER ? "1 Player" : "2 Player";
      const int modeValueWidth = MeasureText(modeValue, paragraphFontSize);
      DrawText(
        modeValue,
        width/2 - modeValueWidth + 175,
        height/2 - paragraphFontSize/2 - 25,
        paragraphFontSize,
        ColorFromHex("#888888")
      );

      // Ball Count selector
      const char* ballCountTitle = "Ball Count (1-3)";  
      const int ballCountTitleWidth = MeasureText(ballCountTitle, paragraphFontSize);
      DrawText(
        ballCountTitle,
        width/2 - 175,
        height/2 - paragraphFontSize/2 + 25,
        paragraphFontSize,
        ColorFromHex("#FFFFFF")
      );  

      const char* ballCountValue = ballCount == ONE_BALL ? "1x" : ballCount == TWO_BALLS ? "2x" : "3x";
      const int ballCountValueWidth = MeasureText(ballCountValue, paragraphFontSize);
      DrawText(
        ballCountValue,
        width/2 - ballCountValueWidth + 175,
        height/2 - paragraphFontSize/2 + 25,
        paragraphFontSize,
        ColorFromHex("#888888")
      );

      // Start hint
      const char* startHint = "Press SPACE to start";
      const int startHintWidth = MeasureText(startHint, paragraphFontSize);
      DrawText(
        startHint,
        width/2 - startHintWidth/2,
        height/2 + 90,
        paragraphFontSize,
        ColorFromHex("#888888")
      );
      break;
    }
    case PLAYING: {
      break;
    }
    case GAME_OVER: {
      break;
    }
  }

  EndDrawing();
}

void Game::shutdown() {
  CloseWindow();
}