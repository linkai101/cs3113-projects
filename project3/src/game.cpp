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
  isRunning(true),
  player(nullptr)
{}

Game::~Game() {
  shutdown();
}

void Game::init() {
  InitWindow(width, height, title);
  SetTargetFPS(60);

  tiles.push_back(new Entity(
    { 24, height - 24.0f }, // position
    { 48, 48 }, // size
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/tiles.png", // textureFilePath
    { 16, 16 }, // textureSpriteSize
    4, // textureSpriteColumns
    3 // textureSpriteIndex
  ));

  tiles.push_back(new Entity(
    { 72, height - 24.0f }, // position
    { 48, 48 }, // size
    0.0f, // rotation
    WHITE, // tint
    "assets/textures/tiles.png", // textureFilePath
    { 16, 16 }, // textureSpriteSize
    4, // textureSpriteColumns
    4 // textureSpriteIndex
  ));

  player = new Player(
    { width / 2.0f, height / 2.0f }
  );
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
  for (Entity* tile : tiles) {
    tile->update(deltaTime);
  }

  player->update(deltaTime);
}

void Game::render() {
  BeginDrawing();

  ClearBackground(ColorFromHex("#000000"));
  
  for (Entity* tile : tiles) {
    tile->render();
  }

  player->render();

  EndDrawing();
}

void Game::shutdown() {
  if (player) {
    delete player;
    player = nullptr;
  }

  for (Entity* tile : tiles) {
    delete tile;
  }
  tiles.clear();

  CloseWindow();
}
