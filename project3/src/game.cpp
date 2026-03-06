#include "game.h"
#include "util/log.h"
#include "util/color.h"
#include <string>
#include <cassert>

Game::Game(
  int width,
  int height,
  const char* title
) :
  width(width),
  height(height),
  title(title),
  isRunning(true)
{}

Game::~Game() {
  shutdown();
}

void Game::init() {
  InitWindow(width, height, title);
  SetTargetFPS(60);
}

void Game::run() {
  // Assert that game has been initialized
  assert(IsWindowReady());

  while (isRunning) {
    processInput();

    float deltaTime = GetFrameTime();
    update(deltaTime);

    render();
  }
}

void Game::processInput() {
  if (WindowShouldClose()) {
    isRunning = false;
  }
}

void Game::update(float deltaTime) {
}

void Game::render() {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));

  EndDrawing();
}

void Game::shutdown() {
  CloseWindow();
}
