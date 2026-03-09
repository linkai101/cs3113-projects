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

  // Create levels
  levels.push_back(new Level(width, height));
  currentLevelIndex = 0;
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

  levels[currentLevelIndex]->processInput();
}

void Game::update(float deltaTime) {
  levels[currentLevelIndex]->update(deltaTime);
}

void Game::render() {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));

  levels[currentLevelIndex]->render();

  EndDrawing();
}

void Game::shutdown() {
  for (Level* level : levels) {
    delete level;
  }
  levels.clear();

  CloseWindow();
}
