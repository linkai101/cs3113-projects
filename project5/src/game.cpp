#include "game.h"
#include "utils/log.h"
#include "utils/color.h"
#include "scenes/world.h"
#include <memory>

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

void Game::init() {
  InitWindow(width, height, title);
  InitAudioDevice();
  SetTargetFPS(60);

  assets.load();

  world = std::make_unique<World>(width, height, assets);
  world->load();
  activeScene = world.get();
}

void Game::run() {
  while (isRunning) {
    processInput();

    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    update(deltaTime);

    render();
  }

  shutdown();
}

void Game::processInput() {
  if (WindowShouldClose()) isRunning = false;

  // Process input for active scene
  if (activeScene) activeScene->processInput();
}

void Game::update(float deltaTime) {
  // Update active scene
  if (activeScene) activeScene->update(deltaTime);
}

void Game::render() const {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));

  // Render active scene
  if (activeScene) activeScene->render();

  EndDrawing();
}

void Game::shutdown() {
  assets.unload();

  CloseAudioDevice();
  CloseWindow();
}