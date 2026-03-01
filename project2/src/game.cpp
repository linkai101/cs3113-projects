#include "game.h"
#include "ai_player.h"
#include "util/log.h"
#include "util/color.h"
#include <string>

Game::Game(
  int width,
  int height,
  const char* title
) :
  width(width),
  height(height),
  title(title),
  isRunning(true),
  gameState(MENU),
  gameMode(TWO_PLAYER),
  ballCount(ONE_BALL),
  balls{nullptr, nullptr, nullptr},
  serveTimers{0.0f, 0.0f, 0.0f}
{
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

  // Create game objects
  wall = new Entity(
    Vector2{ static_cast<float>(width / 2.0f), static_cast<float>(height - 120) },
    Vector2{ 2, 240 },
    0.0f,
    WHITE,
    "assets/textures/wall.png"
  );

  player1 = new Player(
    { width - width / 4.0f, height - 40.0f }, // position
    width / 2.0f + 40.0f, // minX
    width - 40.0f // maxX
  );
  player2 = new Player(
    { width / 4.0f, height - 40.0f }, // position
    40.0f, // minX
    width / 2.0f - 40.0f // maxX
  );

  // Initialize game objects
  wall->init();
  player1->init();
  player2->init();
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
        player1Score = 0;
        player2Score = 0;

        // Recreate player2 based on the selected game mode
        delete player2;
        if (gameMode == ONE_PLAYER) {
          player2 = new AIPlayer(
            { width / 4.0f, height - 40.0f },
            40.0f,
            width / 2.0f - 40.0f,
            width / 2.0f,  // sideMaxX: the screen center
            balls,
            (int)ballCount
          );
        } else {
          player2 = new Player(
            { width / 4.0f, height - 40.0f },
            40.0f,
            width / 2.0f - 40.0f
          );
        }

        for (int i = 0; i < 3; i++) {
          delete balls[i];
          balls[i] = nullptr;
        }
        for (int i = 0; i < ballCount; i++) {
          balls[i] = new Ball({ width / 2.0f, height / 4.0f });
          balls[i]->init();
          serveTimers[i] = i * SERVE_DELAY;
        }
      }

      break;
    }
    case PLAYING: {
      // Handle player 1 movement
      if (IsKeyDown(KEY_LEFT)) {
        player1->move(Player::LEFT);
      }
      if (IsKeyDown(KEY_RIGHT)) {
        player1->move(Player::RIGHT);
      }
      if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
        player1->move(Player::NONE); // stop moving
      }

      // Handle player 2 movement
      if (gameMode == TWO_PLAYER) {
        if (IsKeyDown(KEY_A)) {
          player2->move(Player::LEFT);
        }
        if (IsKeyDown(KEY_D)) {
          player2->move(Player::RIGHT);
        }
        if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
          player2->move(Player::NONE); // stop moving
        }
      }

      break;
    }
    case GAME_OVER: {
      if (IsKeyPressed(KEY_SPACE)) {
        gameState = MENU;
      }
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
      wall->update(deltaTime);
      player1->update(deltaTime);
      player2->update(deltaTime);
      for (int i = 0; i < ballCount; i++) {
        if (serveTimers[i] > 0.0f) {
          serveTimers[i] -= deltaTime;
        } else {
          balls[i]->update(deltaTime);
        }
      }
      checkCollisions();
      break;
    }
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
      // Title
      const char* title = "Volley Pong";
      const int titleWidth = MeasureText(title, HEADING_FONT_SIZE);
      DrawText(
        title,
        width/2 - titleWidth/2,
        height/2 - HEADING_FONT_SIZE - 90,
        HEADING_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      // Game Mode selector
      const char* modeTitle = "Game Mode (T)";  
      const int modeTitleWidth = MeasureText(modeTitle, PARAGRAPH_FONT_SIZE);
      DrawText(
        modeTitle,
        width/2 - 175,
        height/2 - PARAGRAPH_FONT_SIZE/2 - 25,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      const char* modeValue = gameMode == ONE_PLAYER ? "1 Player" : "2 Player";
      const int modeValueWidth = MeasureText(modeValue, PARAGRAPH_FONT_SIZE);
      DrawText(
        modeValue,
        width/2 - modeValueWidth + 175,
        height/2 - PARAGRAPH_FONT_SIZE/2 - 25,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#888888")
      );

      // Ball Count selector
      const char* ballCountTitle = "Ball Count (1-3)";  
      const int ballCountTitleWidth = MeasureText(ballCountTitle, PARAGRAPH_FONT_SIZE);
      DrawText(
        ballCountTitle,
        width/2 - 175,
        height/2 - PARAGRAPH_FONT_SIZE/2 + 25,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );  

      const char* ballCountValue = ballCount == ONE_BALL ? "1x" : ballCount == TWO_BALLS ? "2x" : "3x";
      const int ballCountValueWidth = MeasureText(ballCountValue, PARAGRAPH_FONT_SIZE);
      DrawText(
        ballCountValue,
        width/2 - ballCountValueWidth + 175,
        height/2 - PARAGRAPH_FONT_SIZE/2 + 25,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#888888")
      );

      // Start hint
      const char* startHint = "Press SPACE to start";
      const int startHintWidth = MeasureText(startHint, PARAGRAPH_FONT_SIZE);
      DrawText(
        startHint,
        width/2 - startHintWidth/2,
        height/2 + 90,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#888888")
      );
      break;
    }
    case PLAYING: {
      // Player 1 score
      const std::string player1ScoreText = std::to_string(player1Score);
      const int player1ScoreTextWidth = MeasureText(player1ScoreText.c_str(), SCORE_FONT_SIZE);
      DrawText(
        player1ScoreText.c_str(),
        width - player1ScoreTextWidth - 10,
        10,
        SCORE_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      // Player 2 score
      const std::string player2ScoreText = std::to_string(player2Score);
      const int player2ScoreTextWidth = MeasureText(player2ScoreText.c_str(), SCORE_FONT_SIZE);
      DrawText(
        player2ScoreText.c_str(),
        10,
        10,
        SCORE_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      // Win condition hint
      const char* winConditionText = std::string(std::to_string(WIN_SCORE) + " points to win").c_str();
      const int winConditionTextWidth = MeasureText(winConditionText, PARAGRAPH_FONT_SIZE);
      DrawText(
        winConditionText,
        width/2 - winConditionTextWidth/2,
        10,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#888888")
      );

      // Game objects
      wall->render();
      player1->render();
      player2->render();
      for (int i = 0; i < ballCount; i++) {
        balls[i]->render();
      }
      break;
    }
    case GAME_OVER: {
      // Game over title
      const char* gameOverText = "GAME OVER";
      const int gameOverTextWidth = MeasureText(gameOverText, HEADING_FONT_SIZE);
      DrawText(
        gameOverText,
        width/2 - gameOverTextWidth/2,
        height/2 - HEADING_FONT_SIZE - 40,
        HEADING_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      // Player 1 score
      const std::string player1ScoreText = std::to_string(player1Score);
      const int player1ScoreTextWidth = MeasureText(player1ScoreText.c_str(), SCORE_FONT_SIZE);
      DrawText(
        player1ScoreText.c_str(),
        width/2 - player1ScoreTextWidth/2 + 50,
        height/2 - SCORE_FONT_SIZE/2,
        SCORE_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      // Player 2 score
      const std::string player2ScoreText = std::to_string(player2Score);
      const int player2ScoreTextWidth = MeasureText(player2ScoreText.c_str(), SCORE_FONT_SIZE);
      DrawText(
        player2ScoreText.c_str(),
        width/2 - player2ScoreTextWidth/2 - 50,
        height/2 - SCORE_FONT_SIZE/2,
        SCORE_FONT_SIZE,
        ColorFromHex("#FFFFFF")
      );

      // Play again hint
      const char* playAgainText = "Press SPACE to play again";
      const int playAgainTextWidth = MeasureText(playAgainText, PARAGRAPH_FONT_SIZE);
      DrawText(
        playAgainText,
        width/2 - playAgainTextWidth/2,
        height/2 + 60,
        PARAGRAPH_FONT_SIZE,
        ColorFromHex("#888888")
      );

      break;
    }
  }

  EndDrawing();
}

void Game::shutdown() {
  CloseWindow();
  
  // Clean up game objects
  delete wall;
  wall = nullptr;

  delete player1;
  player1 = nullptr;
  delete player2;
  player2 = nullptr;

  for (int i = 0; i < ballCount; i++) {
    delete balls[i];
    balls[i] = nullptr;
  }
}

void Game::checkGameOver() {
  if (player1Score >= WIN_SCORE || player2Score >= WIN_SCORE) {
    gameState = GAME_OVER;
  }
}

void Game::checkCollisions() {
  // Check collisions for each ball
  for (int i = 0; i < ballCount; i++) {
    Ball* ball = balls[i];
    Rectangle ballRect = ball->getRect();

    // Left/right wall collisions — separate so normals point inward correctly
    if (ballRect.x <= 0) ball->reflect({ 1.0f, 0.0f });
    if (ballRect.x + ballRect.width >= width) ball->reflect({ -1.0f, 0.0f });

    // Center wall collisions
    Rectangle wallRect = wall->getRect();
    if (CheckCollisionRecs(ballRect, wallRect)) {
      float ballCenterX = ballRect.x + ballRect.width / 2.0f;
      float ballCenterY = ballRect.y + ballRect.height / 2.0f;
      float wallCenterX = wallRect.x + wallRect.width / 2.0f;

      if (ballCenterY < wallRect.y) {
        float sign = GetRandomValue(0, 1) == 0 ? -1.0f : 1.0f;
        ball->reflect({ sign * 0.2f, -1.0f }); // hit from above, lean slightly to one side
      } else if (ballCenterX < wallCenterX) {
        ball->reflect({ -1.0f, 0.0f }); // ball on left, normal points left
      } else {
        ball->reflect({ 1.0f, 0.0f });  // ball on right, normal points right
      }
    }

    // Paddle collisions — amplify upward bounce, capped to avoid excessive height
    if (CheckCollisionRecs(ballRect, player1->getRect())) {
      float angle = player1->getRotation() * DEG2RAD;
      ball->reflect({ sinf(angle), -cosf(angle) }, 1.5f, 800.0f);
    }
    if (CheckCollisionRecs(ballRect, player2->getRect())) {
      float angle = player2->getRotation() * DEG2RAD;
      ball->reflect({ sinf(angle), -cosf(angle) }, 1.5f, 800.0f);
    }

    // Bottom, falls off the screen
    if (ballRect.y + ballRect.height >= height) {
      if (ballRect.x < width / 2.0f) {
        player1Score++;
      } else {
        player2Score++;
      }

      checkGameOver();

      // Reset this ball
      ball->reset({ width / 2.0f, height / 4.0f });
    }
  }
}