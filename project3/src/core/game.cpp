#include "game.h"
#include <memory>
#include "utils/log.h"
#include "utils/color.h"

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
  levels.clear();
  CloseWindow();
}

void Game::init() {
  InitWindow(width, height, title);
  SetTargetFPS(60);

  // Create levels
  levels.push_back(std::make_unique<Level>(width, height));
  currentLevelIndex = 0;
}

void Game::run() {
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

