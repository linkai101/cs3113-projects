#include "game.h"
#include "util/log.h"

Game::Game(int width, int height, const char* title)
: width(width), height(height), title(title), isRunning(true) {
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
}

void Game::update(float deltaTime) {}

void Game::render() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  EndDrawing();
}

void Game::shutdown() {
  CloseWindow();
}